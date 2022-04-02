// SPDX-License-Identifier: AGPL-3.0-or-later
/* Chibi - Carla's mini-host plugin loader.
 *
 * Copyright (C) 2022 Alexandros Theodotou <alex at zrythm dot org>
 */

#include "chibi-application.h"

int main(int argc, char **argv) {
  ChibiApplication *app = chibi_application_new();
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
