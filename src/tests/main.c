#include "array_list.h"
#include "camera.h"
#include "util.h"
#include "vector.h"
#include "tests_mesh.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_vector_new(void** state)
{
    {
        vec2 vec = create_vec2();
        vec2 vec_d = create_vec2_data(3, 5);
        assert_true(vec.data[0] == 0 && vec.data[1] == 0);
        assert_true(vec_d.data[0] == 3 && vec_d.data[1] == 5);
    }

    {
        vec3 vec = create_vec3();
        vec3 vec_d = create_vec3_data(3, 5, 7);
        assert_true(vec.data[0] == 0 && vec.data[1] == 0 && vec.data[2] == 0);
        assert_true(vec_d.data[0] == 3 && vec_d.data[1] == 5 && vec_d.data[2] == 7);
    }

    {
        vec4 vec = create_vec4();
        vec4 vec_d = create_vec4_data(3, 5, 7, 9);
        assert_true(vec.data[0] == 0 && vec.data[1] == 0 && vec.data[2] == 0 && vec.data[3] == 0);
        assert_true(vec_d.data[0] == 3 && vec_d.data[1] == 5 && vec_d.data[2] == 7 && vec_d.data[3] == 9);
    }
}

static void test_array_list_new_destroy(void** state)
{
    array_list* list = create_array_list();
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 1);
    array_list_destroy(list);
    assert_null(list);
}

static void test_create_array_list_size(void** state)
{
    array_list* list = create_array_list_size(4);
    assert_non_null(list);
    assert_int_equal(list->length, 0);
    assert_int_equal(list->data_size, 4);
    array_list_destroy(list);
}

static void test_array_list_add(void** state)
{
    array_list* list = create_array_list();
    int i;
    array_list_add(list, 0, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

static void test_array_list_push(void** state)
{
    array_list* list = create_array_list();
    int i;
    array_list_push(list, &i);
    assert_int_equal(list->length, 1);
    array_list_destroy(list);
}

static void test_create_array_list_destroy_deep(void** state)
{
    array_list* list = create_array_list();
    int* i = salloc(sizeof(int));
    array_list_push(list, i);
    array_list_destroy_deep(list);
}

static void test_array_list_index(void** state)
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

static void test_array_list_remove(void** state)
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

static void test_array_list_remove_at(void** state)
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

static void test_array_list_pop(void** state)
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



static void test_camera_2D_new_destroy(void** state)
{
    camera* cam = create_camera_2D();
    assert_non_null(cam);
    assert_non_null(cam->transform);
    // TODO: Test matrix contents
    float x, y, angle, scale_x, scale_y;
    camera_get_position(cam, &x, &y);
    camera_get_angle(cam, &angle);
    camera_get_scale(cam, &scale_x, &scale_y);
    assert_true(x == 0);
    assert_true(y == 0);
    assert_true(angle == 0);
    assert_true(scale_x == 1);
    assert_true(scale_y == 1);
    destroy_camera(cam);
    assert_null(cam);
}

static void test_camera_2D_get_matrix(void** state)
{
    camera* cam = create_camera_2D();
    // TODO: Set specific values in the projection for test purposes
    camera_get_matrix(cam);
    // TODO: Test matrix contents
    destroy_camera(cam);
}

static void test_camera_2D_ortho(void** state)
{
    camera* cam = create_camera_2D();
    camera_get_matrix(cam);
    camera_ortho(cam, 0, 100, 100, 0, 0, 100);
    mat4 mat = create_mat4();
    mat4_ortho(&mat, 0, 100, 100, 0, 0, 100);
    // TODO: Compare matrices
    destroy_camera(cam);
}

static void test_camera_2D_transforms(void** state)
{
    camera* cam = create_camera_2D();
    transform2D* trans = create_transform();
    camera_set_position(cam, 2, 3, false);
    transform_set_position(trans, 2, 3, false);
    // TODO: Compare matrices
    camera_set_angle(cam, 30 * DEGTORAD, false);
    transform_set_angle(trans, 30 * DEGTORAD, false);
    // TODO: Compare matrices
    camera_set_scale(cam, 2, 3, false);
    transform_set_scale(trans, 2, 3, false);
    // TODO: Compare matrices
    destroy_camera(cam);
}

int main(void)
{
    const struct CMUnitTest data_structure_tests[] = {
        cmocka_unit_test(test_array_list_new_destroy),
        cmocka_unit_test(test_create_array_list_size),
        cmocka_unit_test(test_array_list_add),
        cmocka_unit_test(test_array_list_push),
        cmocka_unit_test(test_create_array_list_destroy_deep),
        cmocka_unit_test(test_array_list_index),
        cmocka_unit_test(test_array_list_remove),
        cmocka_unit_test(test_array_list_remove_at),
        cmocka_unit_test(test_array_list_pop),
    };
    const struct CMUnitTest camera_tests[] = {
        cmocka_unit_test(test_camera_2D_new_destroy),
        cmocka_unit_test(test_camera_2D_get_matrix),
        cmocka_unit_test(test_camera_2D_ortho),
        cmocka_unit_test(test_camera_2D_transforms),
    };
    const struct CMUnitTest math_tests[] = {
        cmocka_unit_test(test_vector_new),
    };
    if(cmocka_run_group_tests_name("data structure tests", data_structure_tests, NULL, NULL))
        return 1;
    if(cmocka_run_group_tests_name("camera tests", camera_tests, NULL, NULL))
        return 1;
    if(cmocka_run_group_tests_name("math tests", math_tests, NULL, NULL))
        return 1;

    if(cmocka_run_group_tests_name("mesh tests", mesh_tests, NULL, NULL))
        return 1;
    return 0;
}
