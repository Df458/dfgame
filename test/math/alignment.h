#ifndef TEST_MATH_ALIGNMENT
#define TEST_MATH_ALIGNMENT

#include <CUnit/CUnit.h>

int test_alignment_init();
int test_alignment_cleanup();
void test_alignment_build_tests(CU_pSuite suite);

void test_alignment_origin_2d();
void test_alignment_origin_3d();
void test_alignment_point_2d();
void test_alignment_point_3d();
void test_alignment_box_2d();
void test_alignment_box_3d();

#endif // TEST_MATH_ALIGNMENT
