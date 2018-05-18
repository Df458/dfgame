#include "strings.h"

#include "core/stringutil.h"

int test_strings_init() {
    return 0;
}
int test_strings_cleanup() {
    return 0;
}
void test_strings_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_strings_dup);
    CU_ADD_TEST(suite, test_strings_checks);
}

void test_strings_dup() {
    const char* teststr = "Test Data";

    char* dup = nstrdup(teststr);
    CU_ASSERT_EQUAL(strcmp(teststr, dup), 0);

    sfree(dup)
    CU_ASSERT_EQUAL(nstrdup(dup), NULL);
}
void test_strings_checks() {
    const char* intstr = "45";
    const char* negintstr = "-45";
    const char* floatstr = "10.5";
    const char* invalidstr = "Invalid";

    CU_ASSERT_TRUE(aisi(intstr));
    CU_ASSERT_TRUE(aisi(negintstr));
    CU_ASSERT_FALSE(aisi(floatstr));
    CU_ASSERT_FALSE(aisi(invalidstr));

    CU_ASSERT_TRUE(aisf(intstr));
    CU_ASSERT_TRUE(aisf(negintstr));
    CU_ASSERT_TRUE(aisf(floatstr));
    CU_ASSERT_FALSE(aisf(invalidstr));
}
