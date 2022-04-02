project('cpp-collatz', 'cpp',
  default_options: ['cpp_std=gnu++2a'])

omp = dependency('openmp')

executable('cpp-collatz',
  'main.cpp', 'collatz.cpp',
  dependencies: omp)

