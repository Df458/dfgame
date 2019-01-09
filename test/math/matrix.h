#ifndef TEST_MATH_MATRIX
#define TEST_MATH_MATRIX

#include <CUnit/CUnit.h>

int test_matrix_init();
int test_matrix_cleanup();
void test_matrix_build_tests(CU_pSuite suite);

int test_matrix_general();
int test_matrix_translate();
int test_matrix_scale();
int test_matrix_mul();
int test_matrix_projection();

#endif // TEST_MATH_MATRIX
