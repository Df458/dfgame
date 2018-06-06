#include "utils.h"
#include "core/log/log.h"
#include "math/mathutil.h"

int test_utils_init() {
    return 0;
}
int test_utils_cleanup() {
    return 0;
}
void test_utils_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_utils);
}

void test_utils() {
    CU_ASSERT_EQUAL(clamp(50, 0, 100), 50);
    CU_ASSERT_EQUAL(clamp(-50, 0, 100), 0);
    CU_ASSERT_EQUAL(clamp(150, 0, 100), 100);
    CU_ASSERT_EQUAL(clamp(0.5f, 0, 100), 0.5f);

    CU_ASSERT_EQUAL(min(50, 0), 0);
    CU_ASSERT_EQUAL(min(0, 50), 0);
    CU_ASSERT_EQUAL(min(-50, 0), -50);
    CU_ASSERT_EQUAL(min(-50, -100), -100);
    CU_ASSERT_EQUAL(min(150, 100), 100);
    CU_ASSERT_EQUAL(min(0.5f, 0), 0);

    CU_ASSERT_EQUAL(max(50, 0), 50);
    CU_ASSERT_EQUAL(max(0, 50), 50);
    CU_ASSERT_EQUAL(max(-50, 0), 0);
    CU_ASSERT_EQUAL(max(-50, -100), -50);
    CU_ASSERT_EQUAL(max(150, 100), 150);
    CU_ASSERT_EQUAL(max(0.5f, 0), 0.5f);

    CU_ASSERT_EQUAL(compare(50, 0), COMPARE_GREATER_THAN);
    CU_ASSERT_EQUAL(compare(0, 50), COMPARE_LESS_THAN);
    CU_ASSERT_EQUAL(compare(-50, 0), COMPARE_LESS_THAN);
    CU_ASSERT_EQUAL(compare(-50, -100), COMPARE_GREATER_THAN);
    CU_ASSERT_EQUAL(compare(150, 100), COMPARE_GREATER_THAN);
    CU_ASSERT_EQUAL(compare(0.5f, 0), COMPARE_GREATER_THAN);
    CU_ASSERT_EQUAL(compare(0.5f, 0.5), COMPARE_EQUAL_TO);

    CU_ASSERT_EQUAL(square(5), 25);
    CU_ASSERT_EQUAL(cube(5), 125);
    CU_ASSERT_EQUAL(square(0.5), 0.25);
    CU_ASSERT_EQUAL(cube(0.5), 0.125);
    CU_ASSERT_EQUAL(square(-3), 9);
    CU_ASSERT_EQUAL(cube(-3), -27);
    CU_ASSERT_EQUAL(square(-0.3), 0.09);
    CU_ASSERT_EQUAL(cube(-0.3), -0.027);

    double deg, rad;
    CU_ASSERT_EQUAL(degtorad(0), 0);
    CU_ASSERT_EQUAL(radtodeg(0), 0);

    rad = degtorad(180);
    CU_ASSERT(eq0(PI - rad));
    rad = degtorad(360);
    CU_ASSERT(eq0((PI * 2.0) - rad));

    deg = radtodeg(PI);
    CU_ASSERT(eq0(180 - deg));
    deg = radtodeg(PI * 2.0);
    CU_ASSERT(eq0(360 - deg));

    CU_ASSERT_FALSE(eq0(10));
    CU_ASSERT_FALSE(eq0(-1));
    CU_ASSERT_FALSE(eq0(0.01));
}
