// SPDX-License-Identifier: AGPL-3.0-or-later
/* Chibi - Carla's mini-host plugin loader.
 *
 * Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
 */

#ifndef __CHIBI_APPLICATION_H__
#define __CHIBI_APPLICATION_H__

#include <CarlaHost.h>
#include <CarlaUtils.h>
#include <gtk/gtk.h>

#define CHIBI_APPLICATION_TYPE (chibi_application_get_type())
G_DECLARE_FINAL_TYPE(ChibiApplication, chibi_application, CHIBI, APPLICATION,
                     GtkApplication)

typedef struct _ChibiWindow ChibiWindow;

struct _ChibiApplication {
  GtkApplication parent;

  BinaryType btype;

  PluginType ptype;
  char *ptype_str;

  char *filename;
  char *uri;
  char *name;
  char *label;
  int64_t unique_id;

  const CarlaCachedPluginInfo *cached_plugin_nfo;

  /**
   * Pointer to the GTK thread.
   *
   * Used for checking whether the current thread is
   * the GTK thread.
   */
  const GThread *gtk_thread;

  ChibiWindow *win;
};

ChibiApplication *chibi_application_new(void);

#endif /* __CHIBI_APPLICATION_H__ */
