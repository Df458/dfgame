#include "test_strings.h"

#include "core/stringutil.h"

int test_strings_init() {
    return 0;
}
int test_strings_cleanup() {
    return 0;
}
void test_strings_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_strings_cmp);
    CU_ADD_TEST(suite, test_strings_dup);
    CU_ADD_TEST(suite, test_strings_checks);
}

void test_strings_cmp() {
    const char* str1 = "Test 1";
    const char* str2 = "Test 2";
    const char* str3 = "Test 1";
    const char* nullstr = NULL;

    CU_ASSERT_EQUAL(strcmp(str1, str2), nstrcmp(str1, str2));
    CU_ASSERT_EQUAL(strcmp(str2, str1), nstrcmp(str2, str1));
    CU_ASSERT_EQUAL(strcmp(str1, str3), nstrcmp(str1, str3));

    CU_ASSERT_EQUAL(nstrcmp(str1, nullstr), COMPARE_GREATER_THAN);
    CU_ASSERT_EQUAL(nstrcmp(nullstr, str2), COMPARE_LESS_THAN);
    CU_ASSERT_EQUAL(nstrcmp(nullstr, nullstr), COMPARE_EQUAL_TO);

    CU_ASSERT_EQUAL(strncmp(str1, str2, 6), nstrncmp(str1, str2, 6));
    CU_ASSERT_EQUAL(strncmp(str2, str1, 6), nstrncmp(str2, str1, 6));
    CU_ASSERT_EQUAL(strncmp(str1, str3, 6), nstrncmp(str1, str3, 6));
}
void test_strings_dup() {
    const char* teststr = "Test Data";

    char* dup = nstrdup(teststr);
    CU_ASSERT_EQUAL(strcmp(teststr, dup), 0);
    sfree(dup);

    dup = nstrndup(teststr, 4);
    CU_ASSERT_EQUAL(strcmp("Test", dup), 0);
    sfree(dup);

    dup = nstrndup(teststr, 256);
    CU_ASSERT_EQUAL(strcmp(teststr, dup), 0);
    sfree(dup);

    CU_ASSERT_EQUAL(nstrdup(dup), NULL);
    CU_ASSERT_EQUAL(nstrndup(dup, 4), NULL);
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
