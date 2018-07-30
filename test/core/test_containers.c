#include "test_containers.h"

#include "core/container/array.h"

int compare_ints(void* i1, void* i2, void* user) {
    uint16 u1 = *(uint16*)i1;
    uint16 u2 = *(uint16*)i2;

    if(u1 > u2)
        return COMPARE_GREATER_THAN;
    else if(u1 < u2)
        return COMPARE_LESS_THAN;
    else
        return COMPARE_EQUAL_TO;
}

int test_containers_init() {
    return 0;
}
int test_containers_cleanup() {
    return 0;
}
void test_containers_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_containers_array_basic);
    CU_ADD_TEST(suite, test_containers_array_ordering);
}

void test_containers_array_basic() {
    array a1 = array_new(sizeof(uint16), 10);
    array a2 = array_mnew(uint32, 10);

    CU_ASSERT_PTR_NOT_NULL(a1);
    CU_ASSERT_PTR_NOT_NULL(a2);
    CU_ASSERT_EQUAL(array_get_length(a1), 0);
    CU_ASSERT_EQUAL(array_get_length(a2), 0);

    uint16 i1 = 4;
    uint32 i2 = 8;
    array_add(a1, i1);
    array_add(a2, i1);
    array_add(a1, i2);
    array_add(a2, i2);

    CU_ASSERT_EQUAL(array_get_length(a1), 1);
    CU_ASSERT_EQUAL(array_get_length(a2), 1);

    CU_ASSERT_EQUAL(*(uint16*)array_get(a1, 0), i1);
    CU_ASSERT_EQUAL(*(uint32*)array_get(a2, 0), i2);

    CU_ASSERT_TRUE(array_contains(a1, &i1));
    CU_ASSERT_TRUE(array_contains(a2, &i2));
    CU_ASSERT_FALSE(array_contains(a1, &i2));
    CU_ASSERT_FALSE(array_contains(a2, &i1));

    CU_ASSERT_TRUE(array_remove(a1, &i1));
    CU_ASSERT_TRUE(array_remove(a2, &i2));
    CU_ASSERT_FALSE(array_remove(a1, &i1));
    CU_ASSERT_FALSE(array_remove(a2, &i2));

    CU_ASSERT_EQUAL(array_get_length(a1), 0);
    CU_ASSERT_EQUAL(array_get_length(a2), 0);

    array_set(a1, 0, &i1);
    array_set(a2, 0, &i2);

    CU_ASSERT_EQUAL(array_get_length(a1), 0);
    CU_ASSERT_EQUAL(array_get_length(a2), 0);

    array_add(a1, i1);
    array_add(a2, i2);

    CU_ASSERT_EQUAL(array_get_length(a1), 1);
    CU_ASSERT_EQUAL(array_get_length(a2), 1);

    uint16 i3 = 34;
    uint32 i4 = 46;

    array_add(a1, i3);
    array_add(a2, i4);

    array a3 = array_new(sizeof(uint16), 10);

    array_foreach(a1, it) {
        uint16 n = *(uint16*)it.data;
        array_add(a3, n);
    }
    array_foreach(a2, it) {
        uint16 n = *(uint16*)it.data;
        array_add(a3, n);
    }

    CU_ASSERT_EQUAL(*(uint16*)array_get(a3, 0), i1);
    CU_ASSERT_EQUAL(*(uint16*)array_get(a3, 1), i3);
    CU_ASSERT_EQUAL(*(uint16*)array_get(a3, 2), i2);
    CU_ASSERT_EQUAL(*(uint16*)array_get(a3, 3), i4);

    array_foreach(a3, it) {
        uint16 n = *(uint16*)it.data;
        if(n % 10 == 4) {
            array_remove_iter(a3, &it);
        }
    }

    CU_ASSERT_FALSE(array_contains(a3, &i1));
    CU_ASSERT_TRUE(array_contains(a3, &i2));
    CU_ASSERT_FALSE(array_contains(a3, &i3));
    CU_ASSERT_TRUE(array_contains(a3, &i4));

    array_free(a3);
    CU_ASSERT_FALSE(array_contains(a3, &i4));

    array_free(a2);
    CU_ASSERT_EQUAL(array_get_length(a2), 0);

    array_free(a1);
    CU_ASSERT_EQUAL(array_get(a1, 1), NULL);

}
void test_containers_array_ordering() {
    array a1 = array_new(sizeof(uint16), 10);
    array a2 = array_new_ordered(sizeof(uint16), 10);
    for(uint16 i = 0; i < 10; ++i) {
        array_add(a1, i);
        array_add(a2, i);
    }

    array_remove_at(a1, 0);
    array_remove_at(a2, 0);

    CU_ASSERT_EQUAL(*(uint16*)array_get(a1, 0), 9);
    for(uint16 i = 1; i < 10; ++i) {
        CU_ASSERT_EQUAL(*(uint16*)array_get(a2, i - 1), i);
    }

    array_sort(a1, compare_ints, NULL);
    array_sort(a2, compare_ints, NULL);
    for(uint16 i = 1; i < 10; ++i) {
        CU_ASSERT_EQUAL(*(uint16*)array_get(a1, i - 1), i);
        CU_ASSERT_EQUAL(*(uint16*)array_get(a2, i - 1), i);
    }

    array_remove_at(a1, 1);
    array_remove_at(a2, 1);

    CU_ASSERT_EQUAL(*(uint16*)array_get(a1, 1), 9);
    CU_ASSERT_EQUAL(*(uint16*)array_get(a2, 1), 3);

    array_free(a1);
    array_free(a2);

    array a3 = array_new(sizeof(uint16), 5);
    array a4 = array_new(sizeof(uint16), 5);
    uint16 i1 = 1;
    array_add(a3, i1);
    i1 = 2;
    array_add(a3, i1);
    i1 = 3;
    array_add(a3, i1);
    i1 = 4;
    array_add(a3, i1);
    i1 = 5;
    array_add(a3, i1);

    array_rforeach(a3, it) {
        array_add(a4, *(uint16*)it.data);
    }

    array_foreach(a4, it) {
        uint16 i2 = *(uint16*)it.data;

        CU_ASSERT_EQUAL(i2, i1);

        i1--;
    }
}
