#include "alignment.h"
#include "matrix.h"
#include "utils.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 

int main(int argc, char** argv) {
    CU_initialize_registry();

    test_alignment_build_tests(CU_add_suite("Alignment", test_alignment_init, test_alignment_cleanup));
    test_matrix_build_tests(CU_add_suite("Matrix", test_matrix_init, test_matrix_cleanup));
    test_utils_build_tests(CU_add_suite("Utilities", test_utils_init, test_utils_cleanup));

    CU_basic_run_tests();

    unsigned int failed = CU_get_number_of_tests_failed();

    CU_cleanup_registry();
    return failed ? 1 : 0;
}
