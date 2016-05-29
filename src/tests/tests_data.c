#include <stdbool.h>
#include "tests_data.h"
#include "array_list.h"
#include "sorted_tree.h"
#include "util.h"

void test_array_list_new_destroy(void** state)
{
    array_list* list = create_array_list();
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 1);
    array_list_destroy(list);
    assert_null(list);
}

void test_create_array_list_size(void** state)
{
    array_list* list = create_array_list_size(4);
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 4);
    array_list_destroy(list);
}

void test_array_list_add(void** state)
{
    array_list* list = create_array_list();
    int i;
    array_list_add(list, 0, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

void test_array_list_push(void** state)
{
    array_list* list = create_array_list();
    int i;
    array_list_push(list, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

void test_create_array_list_destroy_deep(void** state)
{
    array_list* list = create_array_list();
    int* i = salloc(sizeof(int));
    array_list_push(list, i);
    array_list_destroy_deep(list);
}

void test_array_list_index(void** state)
{
    array_list* list = create_array_list();
    int i, j, k;
    array_list_push(list, &i);
    array_list_push(list, &j);
    array_list_push(list, &k);
    assert_int_equal(array_list_index(list, &i), 0);
    assert_int_equal(array_list_index(list, &j), 1);
    assert_int_equal(array_list_index(list, &k), 2);
    array_list_destroy(list);
}

void test_array_list_remove(void** state)
{
    array_list* list = create_array_list();
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

void test_array_list_remove_at(void** state)
{
    array_list* list = create_array_list();
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

void test_array_list_pop(void** state)
{
    array_list* list = create_array_list();
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


void test_sorted_tree_new_destroy(void** state)
{
    sorted_tree* tree = create_sorted_tree(10, 0);
    assert_non_null(tree);
    assert_int_equal(tree->balance_factor, 0);
    assert_int_equal(tree->key, 10);
    assert_int_equal(tree->data, 0);
    assert_int_equal(tree->left_child, 0);
    assert_int_equal(tree->right_child, 0);
    destroy_sorted_tree(tree);
    assert_null(tree);
}

void test_sorted_tree_get_set_balance_contains(void** state)
{
    int ptr = 3;
    sorted_tree* tree = create_sorted_tree(10, &ptr);
    sorted_tree_set(tree, 3,  &ptr, true);
    sorted_tree_set(tree, 17, &ptr, true);
    sorted_tree_set(tree, 5,  &ptr, true);
    sorted_tree_set(tree, 20, &ptr, true);
    sorted_tree_set(tree, 2,  &ptr, true);
    sorted_tree_set(tree, 8,  &ptr, true);
    sorted_tree_set(tree, 6,  &ptr, true);
    sorted_tree_set(tree, 4,  &ptr, true);

    assert_non_null(sorted_tree_get(tree, 10));
    assert_non_null(sorted_tree_get(tree, 3));
    assert_non_null(sorted_tree_get(tree, 17));
    assert_non_null(sorted_tree_get(tree, 5));
    assert_non_null(sorted_tree_get(tree, 20));
    assert_true(sorted_tree_contains(tree, 2));
    assert_true(sorted_tree_contains(tree, 8));
    assert_true(sorted_tree_contains(tree, 6));
    assert_true(sorted_tree_contains(tree, 4));

    destroy_sorted_tree(tree);
}

void test_sorted_tree_destroy_deep(void** state)
{
    int* ptr1 = salloc(sizeof(int));
    int* ptr2 = salloc(sizeof(int));
    int* ptr3 = salloc(sizeof(int));
    sorted_tree* tree = create_sorted_tree(10, ptr1);
    sorted_tree_set(tree, 4, ptr2, true);
    sorted_tree_set(tree, 1, ptr3, true);
    destroy_sorted_tree_deep(tree);
}
