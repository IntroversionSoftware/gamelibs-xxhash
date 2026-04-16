project('xxhash', 'c')

incdirs = ['.']

c_compiler = meson.get_compiler('c')
sources = [ 'xxhash.c' ]

if target_machine.cpu_family() == 'x86' or target_machine.cpu_family() == 'x86_64'
  sources += [
      'xxh_x86dispatch.c',
  ]
endif

xxhash_lib = static_library(
  'xxhash',
  sources,
  include_directories: incdirs,
)

xxhash_dep = declare_dependency(
  include_directories: incdirs,
  link_with: [xxhash_lib],
  version: meson.project_version()
)

meson.override_dependency('xxhash', xxhash_dep)
