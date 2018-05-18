test_core_src = [
    'main.c',
    'strings.c'
]
test_core = executable('test_core', test_core_src, include_directories : include_directories('../../src', '../../src/core'), dependencies : [ cunit, coredeps ], link_with : [ corelib ])
test('Core module tests', test_core)
