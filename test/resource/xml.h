#ifndef TEST_RESOURCE_XML
#define TEST_RESOURCE_XML

#include <CUnit/CUnit.h>

int test_xml_init();
int test_xml_cleanup();
void test_xml_build_tests(CU_pSuite suite);

void test_xml_read();
void test_xml_match();

#endif // TEST_RESOURCE_XML
