#ifndef TEST_CORE_CONTAINERS
#define TEST_CORE_CONTAINERS

#include <CUnit/CUnit.h>

int test_containers_init();
int test_containers_cleanup();
void test_containers_build_tests(CU_pSuite suite);

void test_containers_array_basic();
void test_containers_array_ordering();

#endif // TEST_CORE_CONTAINERS
