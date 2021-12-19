project('xxhash', 'cpp')

incdirs = ['.']
libs = []

if target_machine.cpu_family() == 'x86' or target_machine.cpu_family() == 'x86_64'
  c_compiler = meson.get_compiler('c')

  add_project_arguments(c_compiler.get_supported_arguments(['-mno-fma', '-mno-fma4', '-mno-avx2', '-mno-avx', '-mno-bmi', '-mno-bmi2', '-DXXH_DISPATCH_AVX512=0']), language:'c')

  sources = [
	  'xxh_x86dispatch.c',
  ]

  lib = static_library(
	  'xxhash',
	  sources,
	  include_directories: incdirs,
  )

  libs += [lib]
endif

xxhash_dep = declare_dependency(
	include_directories: incdirs,
	link_with: libs,
	version: meson.project_version()
)

meson.override_dependency('xxhash', xxhash_dep)
