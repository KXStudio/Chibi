# SPDX-FileCopyrightText: 2022 Alexandros Theodotou <alex@zrythm.org>
# SPDX-License-Identifier: AGPL-3.0-or-later

chibi_srcs = files ([
  'main.c',
  'chibi-application.c',
  'chibi-window.c',
  ])

chibi_exe = executable (
  'Chibi',
  sources: chibi_srcs,
  dependencies: [
    gtk4_dep,
    carla_host_plugin_dep,
    ],
  install: true)
