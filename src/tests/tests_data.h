#ifndef DF_TESTS_DATA_H
#define DF_TESTS_DATA_H
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_array_list_new_destroy(void** state);
void test_create_array_list_size(void** state);
void test_array_list_add(void** state);
void test_array_list_push(void** state);
void test_create_array_list_destroy_deep(void** state);
void test_array_list_index(void** state);
void test_array_list_remove(void** state);
void test_array_list_remove_at(void** state);
void test_array_list_pop(void** state);

void test_sorted_tree_new_destroy(void** state);
void test_sorted_tree_get_set_balance_contains(void** state);
void test_sorted_tree_destroy_deep(void** state);

static const struct CMUnitTest data_tests[] =
{
    cmocka_unit_test(test_array_list_new_destroy),
    cmocka_unit_test(test_create_array_list_size),
    cmocka_unit_test(test_array_list_add),
    cmocka_unit_test(test_array_list_push),
    cmocka_unit_test(test_create_array_list_destroy_deep),
    cmocka_unit_test(test_array_list_index),
    cmocka_unit_test(test_array_list_remove),
    cmocka_unit_test(test_array_list_remove_at),
    cmocka_unit_test(test_array_list_pop),

    cmocka_unit_test(test_sorted_tree_new_destroy),
    cmocka_unit_test(test_sorted_tree_get_set_balance_contains),
    cmocka_unit_test(test_sorted_tree_destroy_deep),
};

#endif
