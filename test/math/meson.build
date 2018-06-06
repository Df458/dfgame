test_math_src = [
    'main.c',
    'utils.c'
]
test_math = executable('test_math', test_math_src, include_directories : include_directories('../../src'), dependencies : [ cunit, mathdeps ], link_with : [ mathlib ])
test('Math module tests', test_math)
