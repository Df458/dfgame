#ifndef TEST_RESOURCE_PATHS
#define TEST_RESOURCE_PATHS

#include <CUnit/CUnit.h>

int test_paths_init();
int test_paths_cleanup();
void test_paths_build_tests(CU_pSuite suite);

void test_paths_read();

#endif // TEST_RESOURCE_PATHS
