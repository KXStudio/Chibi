// SPDX-License-Identifier: AGPL-3.0-or-later
/* Chibi - Carla's mini-host plugin loader.
 *
 * Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
 * Copyright (C) 2020 Filipe Coelho <falktx@falktx.com>
 */

#include "chibi-application.h"
#include "chibi-window.h"

#include <glib/gi18n.h>

G_DEFINE_TYPE(ChibiApplication, chibi_application, GTK_TYPE_APPLICATION);

static bool print_version(const gchar *option_name, const gchar *value,
                          gpointer data, GError **error) {
  fprintf(stdout, "%s\n", "TODO");

  exit(EXIT_SUCCESS);
}

/**
 * Add the option entries.
 *
 * Things that can be processed immediately should
 * be set as callbacks here (like --version).
 *
 * Things that require to know other options before
 * running should be set as NULL and processed
 * in the handle-local-options handler.
 */
static void add_option_entries(ChibiApplication *self) {
  GOptionEntry entries[] = {
      {"version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
       print_version, _("Print version information"), NULL},
      {"plugin-type", 0, G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE,
       &self->ptype_str, _("Plugin type"), NULL},
      {0},
  };

  g_application_add_main_option_entries(G_APPLICATION(self), entries);
  g_application_set_option_context_parameter_string(G_APPLICATION(self),
                                                    _("[FILE-OR-URI]"));

  char examples[8000];
  sprintf(
      examples,
      _("Examples:\n"
        "  urn:ardour:a-comp                   Open Ardour A-Compressor\n"
        "  -v --version                        Print version information\n\n"
        "Please report issues to %s\n"),
      "https://github.com/KXStudio/Chibi/issues");
  g_application_set_option_context_description(G_APPLICATION(self), examples);

  char summary[8000];
  sprintf(summary, "%s", _("Run Chibi, optionally passing a plugin URI."));
  g_application_set_option_context_summary(G_APPLICATION(self), summary);
}

/**
 * Called on the local instance to handle options.
 */
static int on_handle_local_options(GApplication *app, GVariantDict *opts,
                                   ChibiApplication *self) {
#if 0
  /* example */
  if (g_variant_dict_contains (
        opts, "print-settings"))
    {
      print_settings (self);
    }
#endif

  return -1;
}

ChibiApplication *chibi_application_new(void) {
  ChibiApplication *self =
      g_object_new(CHIBI_APPLICATION_TYPE,
                   /* if an ID is provided, this application
                    * becomes unique (only one instance allowed) */
                   /*"application-id", "studio.kx.Chibi",*/
                   "resource-base-path", "/studio/kx/Chibi", "flags",
                   G_APPLICATION_HANDLES_OPEN, NULL);

  self->gtk_thread = g_thread_self();

  /* add option entries */
  add_option_entries(self);

  /* add handlers */
  g_signal_connect(G_OBJECT(self), "handle-local-options",
                   G_CALLBACK(on_handle_local_options), self);

  return self;
}

static void finalize(ChibiApplication *self) { /* TODO free members */ }

/**
 * First function that gets called afted CLI args
 * are parsed and processed.
 *
 * This gets called before open or activate.
 */
static void on_startup(GApplication *app) {
  ChibiApplication *self = CHIBI_APPLICATION(app);

  fprintf(stdout,
          _("Chibi-%s\n\n"
            "Chibi comes with ABSOLUTELY NO WARRANTY!\n\n"
            "This is free software, and you are welcome to redistribute it\n"
            "under certain conditions. See the LICENSE file for details.\n\n"
            "Write comments and bugs to %s\n\n"),
          "v1", "https://github.com/KXStudio/Chibi/issues");

  char *cur_dir = g_get_current_dir();
  g_message("Running Chibi in %s", cur_dir);
  g_free(cur_dir);

  if (g_getenv("NSM_URL")) {
    /* TODO if under NSM, wait for it to provide a path */
    g_error("not implemented yet");
  }

  self->btype = BINARY_NATIVE;

  /* chain up */
  G_APPLICATION_CLASS(chibi_application_parent_class)
      ->startup(G_APPLICATION(self));
}

/*
 * Called after startup if no filename is passed on
 * command line.
 */
static void on_activate(GApplication *app) {
  ChibiApplication *self = CHIBI_APPLICATION(app);

  /* TODO show selector */
  g_error("unimplemented - must pass at least 1 CLI arg");
}

/**
 * Called when a filename is passed to the command line
 * instead of activate.
 *
 * Always gets called after startup.
 */
static void on_open(GApplication *app, GFile **files, gint n_files,
                    const gchar *hint) {
  ChibiApplication *self = CHIBI_APPLICATION(app);

  if (n_files != 1) {
    g_error("Must pass exactly 1 plugin URI");
  }

  /* get passed URI */
  GFile *file = files[0];
  self->filename = g_file_get_path(file);
  if (!self->filename) {
    self->filename = g_file_get_uri(file);
  }

  /* determine plugin type */
  self->ptype = PLUGIN_LV2;
  if (self->ptype_str) {
    if (g_strcmp0(self->ptype_str, "vst2") == 0) {
      self->ptype = PLUGIN_VST2;
    } else if (g_strcmp0(self->ptype_str, "vst3") == 0) {
      self->ptype = PLUGIN_VST3;
    } else if (g_strcmp0(self->ptype_str, "au") == 0) {
      self->ptype = PLUGIN_AU;
    }
    /* TODO more */
  }

  if (self->ptype == PLUGIN_LV2 || self->ptype == PLUGIN_AU) {
    const unsigned int count = carla_get_cached_plugin_count(self->ptype, NULL);
    if (count > 0) {
      for (unsigned int i = 0; i < count; ++i) {
        const CarlaCachedPluginInfo *pinfo =
            carla_get_cached_plugin_info(self->ptype, i);

        if (!pinfo->valid)
          continue;

        if (self->ptype == PLUGIN_LV2) {
          char **tokens = g_strsplit(pinfo->label, "/", 2);
          char *uri = tokens[1];

          /* this is to make sure that the URI
           * received by glib matches the URI of the
           * plugin in corner cases like `urn:` */
          GFile *uri_file = g_file_new_for_uri(uri);
          uri = g_file_get_uri(uri_file);

          if (g_strcmp0(uri, self->filename) == 0) {
            self->cached_plugin_nfo = pinfo;
            g_free(uri);
            g_strfreev(tokens);
            g_message("found %s", pinfo->label);
            break;
          }
          g_free(uri);
          g_strfreev(tokens);
        }
      }

      if (!self->cached_plugin_nfo) {
        g_error("plugin not found");
      }
    } else {
      g_error("No plugins available, bailing out");
    }

    self->name = g_strdup(self->cached_plugin_nfo->name);
    self->label = g_strdup(self->cached_plugin_nfo->label);

    g_message("opening %s", self->label);
  } else {
    g_error("unimplemented plugin type");
  }

  self->win = chibi_window_new(self);
  gtk_window_present(GTK_WINDOW(self->win));
}

static void chibi_application_class_init(ChibiApplicationClass *class) {
  G_APPLICATION_CLASS(class)->activate = on_activate;
  G_APPLICATION_CLASS(class)->startup = on_startup;
  G_APPLICATION_CLASS(class)->open = on_open;
  /*G_APPLICATION_CLASS (class)->shutdown = on_shutdown;*/

  GObjectClass *klass = G_OBJECT_CLASS(class);

  klass->finalize = (GObjectFinalizeFunc)finalize;
}

static void chibi_application_init(ChibiApplication *self) {}
