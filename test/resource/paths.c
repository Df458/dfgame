#include "paths.h"
#include "resource/paths.h"

int test_paths_init() {
    return 0;
}
int test_paths_cleanup() {
    return 0;
}

void test_paths_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_paths_read);
}

void test_paths_read() {
    const char* pathA = "/home/user/game/assets/test.dat";
    const char* pathB = "/home/user/game/assets/other/other.dat";
    const char* pathC = "/home/user/documents/external.ext.name";
    const char* pathD = "/home/user/game/assets/.hidden.dat";
    const char* pathE = "/home/user/game/assets/";

    CU_ASSERT_EQUAL(strcmp(get_extension(pathA), "dat"), 0);
    CU_ASSERT_EQUAL(strcmp(get_extension(pathB), "dat"), 0);
    CU_ASSERT_EQUAL(strcmp(get_extension(pathC), "ext.name"), 0);
    CU_ASSERT_EQUAL(strcmp(get_extension(pathD), "dat"), 0);
    CU_ASSERT_EQUAL(get_extension(pathE), NULL);
    CU_ASSERT_EQUAL(get_extension(NULL), NULL);

    CU_ASSERT_EQUAL(strcmp(get_filename(pathA, false), "test"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathA, true), "test.dat"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathB, false), "other"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathB, true), "other.dat"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathC, false), "external"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathC, true), "external.ext.name"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathD, false), ".hidden"), 0);
    CU_ASSERT_EQUAL(strcmp(get_filename(pathD, true), ".hidden.dat"), 0);
    CU_ASSERT_EQUAL(get_filename(pathE, false), NULL);
    CU_ASSERT_EQUAL(get_filename(NULL, false), NULL);

    CU_ASSERT_EQUAL(strcmp(get_folder(pathA), "/home/user/game/assets"), 0);
    CU_ASSERT_EQUAL(strcmp(get_folder(pathB), "/home/user/game/assets/other"), 0);
    CU_ASSERT_EQUAL(strcmp(get_folder(pathC), "/home/user/documents"), 0);
    CU_ASSERT_EQUAL(strcmp(get_folder(pathD), "/home/user/game/assets"), 0);
    CU_ASSERT_EQUAL(strcmp(get_folder(pathE), "/home/user/game/assets"), 0);
    CU_ASSERT_EQUAL(get_folder(NULL), NULL);

    CU_ASSERT_EQUAL(strcmp(get_relative_base(pathA, pathB), "/home/user/game/assets/"), 0);
    CU_ASSERT_EQUAL(strcmp(get_relative_base(pathA, pathE), "/home/user/game/assets/"), 0);
    CU_ASSERT_EQUAL(strcmp(get_relative_base(pathA, pathC), "/home/user/"), 0);
    CU_ASSERT_EQUAL(get_relative_base(pathA, NULL), NULL);
}
