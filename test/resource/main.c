#include "xml.h"
#include "paths.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 

int main(int argc, char** argv) {
    CU_initialize_registry();

    test_xml_build_tests(CU_add_suite("XML", test_xml_init, test_xml_cleanup));
    test_paths_build_tests(CU_add_suite("Paths", test_paths_init, test_paths_cleanup));

    CU_basic_run_tests();

    unsigned int failed = CU_get_number_of_tests_failed();

    CU_cleanup_registry();
    return failed ? 1 : 0;
}
