#ifndef TEST_CORE_STRINGS
#define TEST_CORE_STRINGS

#include <CUnit/CUnit.h>

int test_strings_init();
int test_strings_cleanup();
void test_strings_build_tests(CU_pSuite suite);

void test_strings_print();
void test_strings_dup();
void test_strings_checks();

#endif // TEST_CORE_STRINGS
