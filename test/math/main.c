#include "utils.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 

int main(int argc, char** argv) {
    CU_initialize_registry();

    test_utils_build_tests(CU_add_suite("Utilities", test_utils_init, test_utils_cleanup));

    CU_basic_run_tests();

    unsigned int failed = CU_get_number_of_tests_failed();

    CU_cleanup_registry();
    return failed ? 1 : 0;
}
