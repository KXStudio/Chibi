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

#ifndef __CHIBI_WINDOW_H__
#define __CHIBI_WINDOW_H__

#include <gtk/gtk.h>

#include <CarlaNative.h>
#include <CarlaHost.h>

#define CHIBI_WINDOW_TYPE (chibi_window_get_type ())
G_DECLARE_FINAL_TYPE (
  ChibiWindow, chibi_window, CHIBI, WINDOW, GtkApplicationWindow)

typedef struct _ChibiApplication ChibiApplication;

typedef struct _ChibiWindow
{
  GtkWindow       parent_instance;

  CarlaHostHandle host_handle;
  const int       idle_timer;

} ChibiWindow;

ChibiWindow *
chibi_window_new (
  ChibiApplication * app);

#endif /* __CHIBI_WINDOW_H__ */
