// SPDX-FileCopyrightText: 2022 Alexandros Theodotou <alex@zrythm.org>
// SPDX-FileCopyrightText: 2020 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "chibi-window.h"
#include "chibi-application.h"

#include <CarlaBackend.h>

G_DEFINE_TYPE(ChibiWindow, chibi_window, GTK_TYPE_APPLICATION_WINDOW)

static void carla_engine_callback(void *ptr, EngineCallbackOpcode action,
                                  uint plugin_id, int val1, int val2, int val3,
                                  float valf, const char *val_str) {
  ChibiWindow *self = (ChibiWindow *)ptr;

  switch (action) {
  case ENGINE_CALLBACK_PLUGIN_ADDED:
    g_message("Plugin added: %u - %s", plugin_id, val_str);
    break;
  case ENGINE_CALLBACK_PLUGIN_REMOVED:
    g_message("Plugin removed: %u", plugin_id);
    break;
  case ENGINE_CALLBACK_PLUGIN_RENAMED:
    break;
  case ENGINE_CALLBACK_PLUGIN_UNAVAILABLE:
    g_warning("Plugin unavailable: %u - %s", plugin_id, val_str);
    break;
  case ENGINE_CALLBACK_PARAMETER_VALUE_CHANGED:
    /* TODO */
    break;
  case ENGINE_CALLBACK_PARAMETER_DEFAULT_CHANGED:
  case ENGINE_CALLBACK_PARAMETER_MAPPED_CONTROL_INDEX_CHANGED:
  case ENGINE_CALLBACK_PARAMETER_MIDI_CHANNEL_CHANGED:
    break;
  case ENGINE_CALLBACK_OPTION_CHANGED:
    g_message("Option changed: plugin %u - opt %d: %d", plugin_id, val1, val2);
    break;
  case ENGINE_CALLBACK_PROGRAM_CHANGED:
    g_message("Program changed: plugin %u - %d", plugin_id, val1);
    break;
  case ENGINE_CALLBACK_MIDI_PROGRAM_CHANGED:
    g_message("MIDI program changed: plugin %u - %d", plugin_id, val1);
    break;
  case ENGINE_CALLBACK_UI_STATE_CHANGED:
    /* TODO */
    break;
  case ENGINE_CALLBACK_NOTE_ON:
  case ENGINE_CALLBACK_NOTE_OFF:
    break;
  case ENGINE_CALLBACK_UPDATE:
    g_debug("plugin %u needs update", plugin_id);
    break;
  case ENGINE_CALLBACK_RELOAD_INFO:
    g_debug("plugin %u reload info", plugin_id);
    break;
  case ENGINE_CALLBACK_RELOAD_PARAMETERS:
    g_debug("plugin %u reload parameters", plugin_id);
    break;
  case ENGINE_CALLBACK_RELOAD_PROGRAMS:
    g_debug("plugin %u reload programs", plugin_id);
    break;
  case ENGINE_CALLBACK_RELOAD_ALL:
    g_debug("plugin %u reload all", plugin_id);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CLIENT_ADDED:
    g_debug("Patchbay client added: %u plugin %d name %s", plugin_id, val2,
            val_str);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CLIENT_REMOVED:
    g_debug("Patchbay client removed: %u", plugin_id);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CLIENT_RENAMED:
    g_debug("Patchbay client renamed: %u - %s", plugin_id, val_str);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CLIENT_DATA_CHANGED:
    g_debug("Patchbay client data changed: %u - %d %d", plugin_id, val1, val2);
    break;
  case ENGINE_CALLBACK_PATCHBAY_PORT_ADDED: {
    g_debug("PORT ADDED: client %u port %d "
            "group %d name %s",
            plugin_id, val1, val3, val_str);
  } break;
  case ENGINE_CALLBACK_PATCHBAY_PORT_REMOVED:
    g_debug("Patchbay port removed: %u - %d", plugin_id, val1);
    break;
  case ENGINE_CALLBACK_PATCHBAY_PORT_CHANGED:
    g_debug("Patchbay port changed: %u - %d", plugin_id, val1);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CONNECTION_ADDED:
    g_debug("Connection added %s", val_str);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CONNECTION_REMOVED:
    g_debug("Connection removed");
    break;
  case ENGINE_CALLBACK_ENGINE_STARTED:
    g_message("Engine started");
    break;
  case ENGINE_CALLBACK_ENGINE_STOPPED:
    g_message("Engine stopped");
    break;
  case ENGINE_CALLBACK_PROCESS_MODE_CHANGED:
    g_debug("Process mode changed: %d", val1);
    break;
  case ENGINE_CALLBACK_TRANSPORT_MODE_CHANGED:
    g_debug("Transport mode changed: %d - %s", val1, val_str);
    break;
  case ENGINE_CALLBACK_BUFFER_SIZE_CHANGED:
    g_message("Buffer size changed: %d", val1);
    break;
  case ENGINE_CALLBACK_SAMPLE_RATE_CHANGED:
    g_message("Sample rate changed: %f", (double)valf);
    break;
  case ENGINE_CALLBACK_CANCELABLE_ACTION:
    g_debug("Cancelable action: plugin %u - %d - %s", plugin_id, val1, val_str);
    break;
  case ENGINE_CALLBACK_PROJECT_LOAD_FINISHED:
    g_message("Project load finished");
    break;
  case ENGINE_CALLBACK_NSM:
    break;
  /*!
   * Idle frontend.
   * This is used by the engine during long operations that might block the
   * frontend, giving it the possibility to idle while the operation is still in
   * place.
   */
  case ENGINE_CALLBACK_IDLE:
    /* TODO */
    break;
  case ENGINE_CALLBACK_INFO:
    g_message("Engine info: %s", val_str);
    break;
  case ENGINE_CALLBACK_ERROR:
    g_warning("Engine error: %s", val_str);
    break;
  case ENGINE_CALLBACK_QUIT:
    g_warning("Engine quit: engine crashed or "
              "malfunctioned and will no longer work");
    break;
  /*!
   * A plugin requested for its inline display to be redrawn.
   * @a pluginId Plugin Id to redraw
   */
  case ENGINE_CALLBACK_INLINE_DISPLAY_REDRAW:
    /* TODO */
    break;
  /*!
   * A patchbay port group has been added.
   * @a pluginId Client Id
   * @a value1   Group Id (unique value within this client)
   * @a value2   Group hints
   * @a valueStr Group name
   * @see PatchbayPortGroupHints
   */
  case ENGINE_CALLBACK_PATCHBAY_PORT_GROUP_ADDED:
    /* TODO */
    break;
  /*!
   * A patchbay port group has been removed.
   * @a pluginId Client Id
   * @a value1   Group Id
   */
  case ENGINE_CALLBACK_PATCHBAY_PORT_GROUP_REMOVED:
    /* TODO */
    break;
  case ENGINE_CALLBACK_PATCHBAY_PORT_GROUP_CHANGED:
    g_debug("Patchbay port group changed: client %u - "
            "group %d - hints %d - name %s",
            plugin_id, val1, val2, val_str);
    break;
  case ENGINE_CALLBACK_PARAMETER_MAPPED_RANGE_CHANGED:
    g_debug("Parameter mapped range changed: %u:%d "
            "- %s",
            plugin_id, val1, val_str);
    break;
  case ENGINE_CALLBACK_PATCHBAY_CLIENT_POSITION_CHANGED:
    break;
  case ENGINE_CALLBACK_EMBED_UI_RESIZED:
    g_debug("Embed UI resized: %u - %dx%d", plugin_id, val1, val2);
    break;
  default:
    break;
  }
}

static const char *carla_file_callback(void *const ptr,
                                       const FileCallbackOpcode action,
                                       const bool is_dir,
                                       const char *const title,
                                       const char *const filter) {

  return NULL;
}

const char *get_gsk_renderer_type(void) {
  static const char *renderer_type = NULL;
  if (renderer_type)
    return renderer_type;

  GdkSurface *surface = gdk_surface_new_toplevel(gdk_display_get_default());
  GskRenderer *renderer = gsk_renderer_new_for_surface(surface);
  if (g_strcmp0(G_OBJECT_TYPE_NAME(renderer), "GskGLRenderer") == 0) {
    renderer_type = "GL";
  } else if (g_strcmp0(G_OBJECT_TYPE_NAME(renderer), "GskCairoRenderer") == 0) {
    renderer_type = "Cairo";
  } else {
    g_warning("unknown renderer");
    renderer_type = "Unknown";
  }

  gsk_renderer_unrealize(renderer);
  g_object_unref(renderer);
  gdk_surface_destroy(surface);

  return renderer_type;
}

static GdkGLContext *clear_gl_context(void) {
  if (g_strcmp0(get_gsk_renderer_type(), "GL") == 0) {
    GdkGLContext *context = gdk_gl_context_get_current();
    if (context) {
      g_object_ref(context);
      gdk_gl_context_clear_current();
    }
    return context;
  }

  return NULL;
}

static void return_gl_context(GdkGLContext *context) {
  if (context) {
    gdk_gl_context_make_current(context);
    g_object_unref(context);
  }
}

/**
 * Tick callback for the plugin UI.
 */
static gboolean idle_cb(gpointer user_data) {
  ChibiWindow *self = (ChibiWindow *)user_data;
  GdkGLContext *context = clear_gl_context();
  carla_engine_idle(self->host_handle);
  return_gl_context(context);

  return G_SOURCE_CONTINUE;
}

static void idle_cb_destroy(gpointer user_data) {
  g_return_if_fail(user_data);
  ChibiWindow *self = (ChibiWindow *)user_data;

  self->idle_source_id = 0;
}

ChibiWindow *chibi_window_new(ChibiApplication *app) {
  g_return_val_if_fail(app->name, NULL);

  char *proper_name = g_strdup_printf(
      "Chibi - %s%s", g_ascii_isdigit(app->name[0]) ? "-" : "", app->name);

  ChibiWindow *self = g_object_new(CHIBI_WINDOW_TYPE, "application", app,
                                   "title", proper_name, NULL);

  self->host_handle = carla_standalone_host_init();
  if (!self->host_handle) {
    g_error("failed to create host handle");
  }

  carla_set_engine_option(self->host_handle, ENGINE_OPTION_OSC_ENABLED, 0,
                          NULL);

  GdkRGBA fg_color, bg_color;
  gdk_rgba_parse(&fg_color, "white");
  gdk_rgba_parse(&bg_color, "black");

  carla_set_engine_callback(self->host_handle, carla_engine_callback, self);
  carla_set_file_callback(self->host_handle, carla_file_callback, self);

  // TODO set frontendWinId in case we cannot embed

  carla_set_engine_option(self->host_handle, ENGINE_OPTION_PREFER_UI_BRIDGES, 0,
                          NULL);

  /* check available drivers */
  uint driver_count = carla_get_engine_driver_count();
  for (uint i = 0; i < driver_count; i++) {
    g_message("found driver %u: %s", i, carla_get_engine_driver_name(i));
  }

  carla_set_engine_option(self->host_handle, ENGINE_OPTION_PROCESS_MODE,
                          ENGINE_PROCESS_MODE_CONTINUOUS_RACK, NULL);
  carla_set_engine_option(self->host_handle, ENGINE_OPTION_TRANSPORT_MODE,
                          ENGINE_TRANSPORT_MODE_INTERNAL, NULL);
  /* using dummy driver for now */
  bool ret = carla_engine_init(self->host_handle, "Dummy", "Chibi");
  if (!ret) {
    g_error("failed to init engine: %s",
            carla_get_last_error(self->host_handle));
  }

  ret = false;
  switch (app->ptype) {
  case PLUGIN_LV2:
    ret = carla_add_plugin(self->host_handle, app->btype, app->ptype, NULL,
                           app->name, app->uri, 0, NULL, PLUGIN_OPTIONS_NULL);
    break;
  default:
    g_error("not implemented");
    break;
  }
  if (!ret) {
    g_error("Failed to add plugin, error was: %s",
            carla_get_last_error(self->host_handle));
    return NULL;
  }

  /* TODO */
#if 0
    void * const ptr =
      carla_embed_custom_ui (
        self->host_handle, 0,
        (void*)(intptr_t)ui->embedwidget->winId());
    ui->embedwidget->setup(ptr);

    if (ui->embedwidget->wasResized())
        resize(ui->embedwidget->size());
    else
        adjustSize();
#endif

  /* show UI (FIXME this creates a new window managed by
   * carla - switch to embed) */
  GdkGLContext *context = clear_gl_context();
  carla_show_custom_ui(self->host_handle, 0, true);
  return_gl_context(context);

  /* do not use tick callback: */
  /* falktx: I am doing some checks on
   * ildaeil/carla, and see there is a nice
   * way without conflicts to avoid the GL
   * context issues. it came from cardinal,
   * where I cannot draw plugin UIs in the
   * same function as the main stuff,
   * because it is in between other opengl
   * calls (before and after). the solution
   * I found was to have a dedicated idle
   * timer, and handle the plugin UI stuff
   * there, outside of the main application
   * draw function */
  self->idle_source_id =
      g_timeout_add_full(G_PRIORITY_DEFAULT,
                         /* 60 fps */
                         1000 / 60, idle_cb, self, idle_cb_destroy);

  return self;
}

static void chibi_window_class_init(ChibiWindowClass *klass) {}

static void chibi_window_init(ChibiWindow *self) {}
