#ifndef TEST_CORE_STRINGS
#define TEST_CORE_STRINGS

#include <CUnit/CUnit.h>

int test_strings_init();
int test_strings_cleanup();
void test_strings_build_tests(CU_pSuite suite);

void test_strings_cmp();
void test_strings_dup();
void test_strings_checks();
void test_strings_len();
void test_strings_split();

#endif // TEST_CORE_STRINGS
