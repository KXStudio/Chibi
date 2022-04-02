chibi_srcs = files ([
  'main.c',
  'chibi-application.c',
  ])

chibi_exe = executable (
  'Chibi',
  sources: chibi_srcs,
  dependencies: [
    gtk4_dep,
    carla_host_plugin_dep,
    ],
  install: true)
