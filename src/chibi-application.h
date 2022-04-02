/*
 * Chibi - Carla's mini-host plugin loader
 *
 * Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Chibi
 *
 * Chibi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chibi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Chibi.  If not, see <https://www.gnu.org/licenses/>.
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
