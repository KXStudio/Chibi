project (
  'Chibi', 'c',
  version: '0.1',
  license: 'AGPL-3-or-later',
  default_options: [
    'warning_level=2',
    'buildtype=debugoptimized',
    'c_std=c11',
    'wrap_mode=nofallback',
    ],
  )

gtk4_dep = dependency (
  'gtk4',
  fallback: ['gtk4', 'libgtk_dep'],
  default_options: [
    'demos=false', 'build-examples=false',
    'build-tests=false',
    'media-gstreamer=disabled',
    'introspection=disabled'])
carla_host_plugin_dep = dependency ('carla-host-plugin')

subdir ('src')
