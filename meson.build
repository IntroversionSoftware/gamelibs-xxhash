project('xxhash', 'c')

global_c_flags = []
c_flags = []
incdirs = ['.']
libs = []

if target_machine.cpu_family() == 'x86' or target_machine.cpu_family() == 'x86_64'
  c_compiler = meson.get_compiler('c')

  c_flags += c_compiler.get_supported_arguments([
    # Require SSE4.1 + SSE4.2 by default
    '-march=x86-64-v2',

    # Disable these ISAs because they'll be executed by dispatch instead.
    '-mno-avx',
    '-mno-avx2',
    '-mno-fma',
    '-mno-fma4',
    '-mno-bmi',
    '-mno-bmi2',
    '-mno-avx512',
    '-mno-avx512f',
    '-mno-avx10.1-256',
  ])

  global_c_flags += [
    '-DXXH_DISPATCH_AVX512=0'
  ]

  sources = [
    'xxh_x86dispatch.c',
  ]

  lib = static_library(
    'xxhash',
    sources,
    c_args: c_flags + global_c_flags,
    include_directories: incdirs,
  )

  libs += [lib]
endif

xxhash_dep = declare_dependency(
  compile_args: global_c_flags,
  include_directories: incdirs,
  link_with: libs,
  version: meson.project_version()
)

meson.override_dependency('xxhash', xxhash_dep)

# vim: set ts=2 sts=2 sw=2 et:
