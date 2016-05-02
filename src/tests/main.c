#include "array_list.h"
#include "util.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_array_list_new_destroy(void** state)
{
    array_list* list = array_list_new();
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 1);
    array_list_destroy(list);
    assert_null(list);
}

static void test_array_list_new_size(void** state)
{
    array_list* list = array_list_new_size(4);
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 4);
    array_list_destroy(list);
}

static void test_array_list_add(void** state)
{
    array_list* list = array_list_new();
    int i;
    array_list_add(list, 0, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

static void test_array_list_push(void** state)
{
    array_list* list = array_list_new();
    int i;
    array_list_push(list, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

static void test_array_list_new_destroy_deep(void** state)
{
    array_list* list = array_list_new();
    int* i = salloc(sizeof(int));
    array_list_push(list, i);
    array_list_destroy_deep(list);
}

static void test_array_list_index(void** state)
{
    array_list* list = array_list_new();
    int i, j, k;
    array_list_push(list, &i);
    array_list_push(list, &j);
    array_list_push(list, &k);
    assert_int_equal(array_list_index(list, &i), 0);
    assert_int_equal(array_list_index(list, &j), 1);
    assert_int_equal(array_list_index(list, &k), 2);
    array_list_destroy(list);
}

static void test_array_list_remove(void** state)
{
    array_list* list = array_list_new();
    int i, j, k;
    array_list_push(list, &i);
    array_list_push(list, &j);
    array_list_push(list, &k);
    array_list_remove(list, &j);
    assert_int_equal(list->length, 2);
    assert_int_equal(array_list_index(list, &i), 0);
    assert_int_equal(array_list_index(list, &k), 1);
    array_list_destroy(list);
}

static void test_array_list_remove_at(void** state)
{
    array_list* list = array_list_new();
    int i, j, k;
    array_list_push(list, &i);
    array_list_push(list, &j);
    array_list_push(list, &k);
    array_list_remove_at(list, 1);
    assert_int_equal(list->length, 2);
    assert_int_equal(array_list_index(list, &i), 0);
    assert_int_equal(array_list_index(list, &k), 1);
    array_list_destroy(list);
}

static void test_array_list_pop(void** state)
{
    array_list* list = array_list_new();
    int i, j, k;
    array_list_push(list, &i);
    array_list_push(list, &j);
    array_list_push(list, &k);
    array_list_pop(list);
    assert_int_equal(list->length, 2);
    assert_int_equal(array_list_index(list, &i), 0);
    assert_int_equal(array_list_index(list, &j), 1);
    array_list_destroy(list);
}

int main(void)
{
    const struct CMUnitTest data_structure_tests[] = {
        cmocka_unit_test(test_array_list_new_destroy),
        cmocka_unit_test(test_array_list_new_size),
        cmocka_unit_test(test_array_list_add),
        cmocka_unit_test(test_array_list_push),
        cmocka_unit_test(test_array_list_new_destroy_deep),
        cmocka_unit_test(test_array_list_index),
        cmocka_unit_test(test_array_list_remove),
        cmocka_unit_test(test_array_list_remove_at),
        cmocka_unit_test(test_array_list_pop),
    };

    return cmocka_run_group_tests_name("data structure tests", data_structure_tests, NULL, NULL);
}
