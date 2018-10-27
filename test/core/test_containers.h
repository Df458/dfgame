#ifndef TEST_CORE_CONTAINERS
#define TEST_CORE_CONTAINERS

#include <CUnit/CUnit.h>

int test_containers_init();
int test_containers_cleanup();
void test_containers_build_tests(CU_pSuite suite);

void test_containers_array_basic();
void test_containers_array_iter_delete();
void test_containers_array_ordering();

void test_containers_hashmap_basic();
void test_containers_hashmap_iter();
void test_containers_hashmap_iter_delete();

#endif // TEST_CORE_CONTAINERS
