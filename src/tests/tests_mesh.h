#ifndef DF_TESTS_MESH_H
#define DF_TESTS_MESH_H
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_mesh_new_destroy(void** state);

static const struct CMUnitTest mesh_tests[] =
{
    cmocka_unit_test(test_mesh_new_destroy),
};

#endif
