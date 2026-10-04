project(
  'mnist-handwritten-digit-recognizer',
  'cpp',
  default_options: [
    'cpp_std=c++20',
    'optimization=3',
    #'b_sanitize=address,undefined',
    'warning_level=2'
  ]
)

openmp = dependency('openmp')
tclap = dependency('tclap')

add_project_arguments([ '-march=native', '-mtune=native' ], language: 'cpp')

sources = []

subdir('src')

executable('mnist-handwritten-digit-recognizer', sources, dependencies : [ openmp, tclap ])
