#include "test_strings.h"

#include "core/stringutil.h"
#include "core/log/log.h"

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
    CU_ADD_TEST(suite, test_strings_len);
    CU_ADD_TEST(suite, test_strings_split);
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

void test_strings_len() {
    const char* str1 = NULL;
    const char* str2 = "";
    const char* str3 = "test";
    const char* str4 = "1234567890";

    CU_ASSERT_TRUE(nstrlen(str1) == 0);
    CU_ASSERT_TRUE(nstrlen(str2) == 0);
    CU_ASSERT_TRUE(nstrlen(str3) == 4);
    CU_ASSERT_TRUE(nstrlen(str4) == 10);

    CU_ASSERT_TRUE(nstrnlen(str1, 5) == 0);
    CU_ASSERT_TRUE(nstrnlen(str2, 5) == 0);
    CU_ASSERT_TRUE(nstrnlen(str3, 5) == 4);
    CU_ASSERT_TRUE(nstrnlen(str4, 5) == 5);

    CU_ASSERT_TRUE(nstrempty(str1));
    CU_ASSERT_TRUE(nstrempty(str2));
    CU_ASSERT_FALSE(nstrempty(str3));
    CU_ASSERT_FALSE(nstrempty(str4));
}

void test_strings_split() {
    const char* str1 = "I think this is a test";
    const char* str2 = "   This    is  a      test  ";
    const char* str3 = NULL;
    const char* str4 = "";

    const char* delim1 = " ";
    const char* delim2 = "is";
    const char* delim3 = NULL;
    const char* delim4 = "";
    const char* delim5 = "Bulldog";

    char** result;
    uint16 count;

    // Empty string
    CU_ASSERT_EQUAL(result = nstrsplit(str3, delim1, &count), NULL);
    CU_ASSERT_EQUAL(count, 0);
    CU_ASSERT_EQUAL(result = nstrsplit(str4, delim1, &count), NULL);
    CU_ASSERT_EQUAL(count, 0);

    // Empty delimiter
    CU_ASSERT_EQUAL(result = nstrsplit(str1, delim3, &count), NULL);
    CU_ASSERT_EQUAL(count, 0);
    CU_ASSERT_EQUAL(result = nstrsplit(str1, delim4, &count), NULL);
    CU_ASSERT_EQUAL(count, 0);

    // Missing delimiter
    CU_ASSERT_NOT_EQUAL(result = nstrsplit(str1, delim5, &count), NULL);
    CU_ASSERT_EQUAL(count, 1);
    CU_ASSERT_TRUE(nstreq(result[0], str1));

    // Correct
    CU_ASSERT_NOT_EQUAL(result = nstrsplit(str1, delim1, &count), NULL);
    CU_ASSERT_EQUAL(count, 6);
    CU_ASSERT_TRUE(nstreq(result[0], "I"));
    CU_ASSERT_TRUE(nstreq(result[1], "think"));
    CU_ASSERT_TRUE(nstreq(result[2], "this"));
    CU_ASSERT_TRUE(nstreq(result[3], "is"));
    CU_ASSERT_TRUE(nstreq(result[4], "a"));
    CU_ASSERT_TRUE(nstreq(result[5], "test"));

    CU_ASSERT_NOT_EQUAL(result = nstrsplit(str2, delim1, &count), NULL);
    CU_ASSERT_EQUAL(count, 4);
    CU_ASSERT_TRUE(nstreq(result[0], "This"));
    CU_ASSERT_TRUE(nstreq(result[1], "is"));
    CU_ASSERT_TRUE(nstreq(result[2], "a"));
    CU_ASSERT_TRUE(nstreq(result[3], "test"));

    CU_ASSERT_NOT_EQUAL(result = nstrsplit(str1, delim2, &count), NULL);
    CU_ASSERT_EQUAL(count, 5);
    CU_ASSERT_TRUE(nstreq(result[0], "I th"));
    CU_ASSERT_TRUE(nstreq(result[1], "nk th"));
    CU_ASSERT_TRUE(nstreq(result[2], " "));
    CU_ASSERT_TRUE(nstreq(result[3], " a te"));
    CU_ASSERT_TRUE(nstreq(result[4], "t"));

    CU_ASSERT_NOT_EQUAL(result = nstrsplit(str2, delim2, &count), NULL);
    CU_ASSERT_EQUAL(count, 4);
    CU_ASSERT_TRUE(nstreq(result[0], "   Th"));
    CU_ASSERT_TRUE(nstreq(result[1], "    "));
    CU_ASSERT_TRUE(nstreq(result[2], "  a      te"));
    CU_ASSERT_TRUE(nstreq(result[3], "t  "));
}
