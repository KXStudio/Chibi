project (
  'Chibi', 'c',
  version: '0.1',
  license: 'AGPL-3-or-later',
  default_options: [
    'warning_level=2',
    'buildtype=debugoptimized',
    'c_std=c11',
    ],
  )

gtk4_dep = dependency ('gtk4')
carla_host_plugin_dep = dependency ('carla-host-plugin')

subdir ('src')
