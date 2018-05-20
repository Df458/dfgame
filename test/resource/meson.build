test_resource_src = [
    'main.c',
    'paths.c',
    'xml.c'
]
test_resource = executable('test_resource', test_resource_src, include_directories : include_directories('../../src', '../../src/core'), dependencies : [ cunit, resourcedeps ], link_with : [ corelib, resourcelib ])
test('Resource module tests', test_resource)
