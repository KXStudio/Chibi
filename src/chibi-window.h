// SPDX-FileCopyrightText: 2022 Alexandros Theodotou <alex@zrythm.org>
// SPDX-License-Identifier: AGPL-3.0-or-later

#ifndef __CHIBI_WINDOW_H__
#define __CHIBI_WINDOW_H__

#include <gtk/gtk.h>

#include <CarlaHost.h>
#include <CarlaNative.h>

#define CHIBI_WINDOW_TYPE (chibi_window_get_type())
G_DECLARE_FINAL_TYPE(ChibiWindow, chibi_window, CHIBI, WINDOW,
                     GtkApplicationWindow)

typedef struct _ChibiApplication ChibiApplication;

typedef struct _ChibiWindow {
  GtkWindow parent_instance;

  CarlaHostHandle host_handle;
  const int idle_timer;

  guint idle_source_id;

} ChibiWindow;

ChibiWindow *chibi_window_new(ChibiApplication *app);

#endif /* __CHIBI_WINDOW_H__ */
