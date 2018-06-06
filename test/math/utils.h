#ifndef TEST_MATH_UTILS
#define TEST_MATH_UTILS

#include <CUnit/CUnit.h>

int test_utils_init();
int test_utils_cleanup();
void test_utils_build_tests(CU_pSuite suite);

void test_utils();

#endif // TEST_MATH_UTILS
