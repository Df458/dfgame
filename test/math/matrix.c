#include "matrix.h"

#include "core/log/log.h"
#include "math/matrix.h"

#include <stdlib.h>
#include <time.h>

#define mat4_eq(a, b) { \
    info(mat4_printstr, mat4_printargs(a));\
    info(mat4_printstr, mat4_printargs(b));\
    for(int i = 0; i < 16; ++i) { \
        CU_ASSERT_DOUBLE_EQUAL(a.data[i], b.data[i], 0.001); \
    } \
}

int test_matrix_init() {
    srand(time(NULL));
    return 0;
}
int test_matrix_cleanup() {
    return 0;
}
void test_matrix_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_matrix_general);
    CU_ADD_TEST(suite, test_matrix_translate);
    CU_ADD_TEST(suite, test_matrix_scale);
    CU_ADD_TEST(suite, test_matrix_mul);
    CU_ADD_TEST(suite, test_matrix_projection);
}

int test_matrix_general() {
    mat4 m1 = mat4_ident;
    mat4 m2 = {{
        1, 1, 2, 3,
        1, 1, 4, 5,
        2, 4, 1, 6,
        3, 5, 6, 1
    }};
    mat4 m3 = {{
        1, 2, 3, 4,
        4, 3, 2, 1,
        1, 3, 2, 4,
        4, 2, 3, 1
    }};

    CU_ASSERT(eq0(mat4_determinant(m1) - 1.0f));
    CU_ASSERT(eq0(mat4_determinant(m2) - 44.0f));
    CU_ASSERT(eq0(mat4_determinant(m3)));

    mat4 b = mat4_transpose(m1);
    mat4_eq(m1, b);
    b = mat4_transpose(m2);
    mat4_eq(m2, b);
    b = mat4_transpose(m3);
    mat4 c = {{
        1, 4, 1, 4,
        2, 3, 3, 2,
        3, 2, 2, 3,
        4, 1, 4, 1
    }};
    mat4_eq(c, b);

    b = mat4_invert(m1);
    mat4_eq(m1, b);

    b = mat4_invert(m2);
    c = (mat4){{
        164, -74, -20, -2,
        -74,  24,  16,  6,
        -20,  16, -4,   4,
        -2,   6,   4,  -4
    }};
    for(int i = 0; i < 16; ++i) {
        c.data[i] /= 44.0f;
    }

    mat4_eq(c, b);
    b = mat4_invert(m3);
    mat4_eq(mat4_ident, b); // Cannot invert

    return 0;
}
int test_matrix_translate() {
    mat4 m1 = mat4_ident;
    mat4 m2 = {{
        1, 1, 2, 3,
        1, 1, 4, 5,
        2, 4, 1, 6,
        0, 0, 0, 1
    }};
    mat4 m3 = {{
        1, 2, 3, 4,
        4, 3, 2, 1,
        1, 3, 2, 4,
        4, 2, 3, 1
    }};

    vec4 v1 = vec4_zero;
    vec4 v2 = { .x = rand() % 10, .y = rand() % 10, .z = rand() % 10, .w = rand() % 10 };

    mat4 b = mat4_translate(m1, v1.xy);
    mat4_eq(b, m1);
    b = mat4_translate(m2, v1.xy);
    mat4_eq(b, m2);
    b = mat4_translate(m3, v1.xy);
    mat4_eq(b, m3);
    b = mat4_translate(m1, v1.xyz);
    mat4_eq(b, m1);
    b = mat4_translate(m2, v1.xyz);
    mat4_eq(b, m2);
    b = mat4_translate(m3, v1.xyz);
    mat4_eq(b, m3);

    b = mat4_translate(m1, v2.xy);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], 0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);
    b = mat4_translate(m2, v2.xy);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], 0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);
    b = mat4_translate(m3, v2.xy);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x + 4, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y + 2, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], 3, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);
    b = mat4_translate(m1, v2.xyz);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], v2.z, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);
    b = mat4_translate(m2, v2.xyz);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], v2.z, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);
    b = mat4_translate(m3, v2.xyz);
    CU_ASSERT_DOUBLE_EQUAL(b.data[12], v2.x + 4, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[13], v2.y + 2, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[14], v2.z + 3, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(b.data[15], 1, 0.001);

    return 0;
}
int test_matrix_scale() {
    mat4 m1 = mat4_ident;
    mat4 m2 = {{
        1, 1, 2, 3,
        1, 1, 4, 5,
        2, 4, 1, 6,
        3, 5, 6, 1
    }};
    mat4 m3 = {{
        1, 1, 2, 3,
        1, 2, 4, 5,
        2, 4, 3, 6,
        3, 5, 6, 4
    }};

    vec4 v1 = { .x=1, .y=1, .z=1, .w=1 };
    vec4 v2 = { .x = rand() % 10, .y = rand() % 10, .z = rand() % 10, .w = rand() % 10 };

    mat4 b = mat4_scale(m1, v1);
    mat4_eq(m1, b);
    b = mat4_scale(m2, v1);
    mat4_eq(m2, b);
    b = mat4_scale(m3, v1);
    mat4_eq(m3, b);

    b = mat4_scale(m1, v2.xy);
    mat4 c = {{
        v2.x, 0, 0, 0,
        0, v2.y, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    }};
    mat4_eq(c, b);
    b = mat4_scale(m2, v2.xy);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y, 4, 5,
        2, 4, 1, 6,
        3, 5, 6, 1,
    }};
    mat4_eq(c, b);
    b = mat4_scale(m3, v2.xy);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y * 2, 4, 5,
        2, 4, 3, 6,
        3, 5, 6, 4,
    }};

    b = mat4_scale(m1, v2.xyz);
    c = (mat4){{
        v2.x, 0, 0, 0,
        0, v2.y, 0, 0,
        0, 0, v2.z, 0,
        0, 0, 0, 1,
    }};
    mat4_eq(c, b);
    b = mat4_scale(m2, v2.xyz);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y, 4, 5,
        2, 4, v2.z, 6,
        3, 5, 6, 1,
    }};
    mat4_eq(c, b);
    b = mat4_scale(m3, v2.xyz);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y * 2, 4, 5,
        2, 4, v2.z * 3, 6,
        3, 5, 6, 4,
    }};
    mat4_eq(c, b);

    b = mat4_scale(m1, v2);
    c = (mat4){{
        v2.x, 0, 0, 0,
        0, v2.y, 0, 0,
        0, 0, v2.z, 0,
        0, 0, 0, v2.w,
    }};
    mat4_eq(c, b);
    b = mat4_scale(m2, v2);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y, 4, 5,
        2, 4, v2.z, 6,
        3, 5, 6, v2.w,
    }};
    mat4_eq(c, b);
    b = mat4_scale(m3, v2);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.y * 2, 4, 5,
        2, 4, v2.z * 3, 6,
        3, 5, 6, v2.w * 4,
    }};
    mat4_eq(c, b);

    b = mat4_scale(m1, v2.x);
    c = (mat4){{
        v2.x, 0, 0, 0,
        0, v2.x, 0, 0,
        0, 0, v2.x, 0,
        0, 0, 0, 1
    }};
    mat4_eq(c, b);
    b = mat4_scale(m2, v2.x);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.x, 4, 5,
        2, 4, v2.x, 6,
        3, 5, 6, 1
    }};
    mat4_eq(c, b);
    b = mat4_scale(m3, v2.x);
    c = (mat4){{
        v2.x, 1, 2, 3,
        1, v2.x * 2, 4, 5,
        2, 4, v2.x * 3, 6,
        3, 5, 6, 4,
    }};
    mat4_eq(c, b);

    return 0;
}
int test_matrix_mul() {
    mat4 m1 = mat4_ident;
    mat4 m2 = {{
        1, 1, 2, 3,
        1, 1, 4, 5,
        2, 4, 1, 6,
        3, 5, 6, 1
    }};
    mat4 m3 = {{
        1, 1, 2, 3,
        1, 2, 4, 5,
        2, 4, 3, 6,
        3, 5, 6, 4
    }};

    mat4_eq(mat4_mul(m1, m2), m2);
    mat4_eq(mat4_mul(m1, m3), m3);
    mat4_eq(mat4_mul(m2, m1), m2);
    mat4_eq(mat4_mul(m3, m1), m3);

    mat4 c = {{
        15, 26, 30, 32,
        25, 44, 48, 52,
        26, 44, 59, 56,
        23, 42, 50, 74
    }};
    mat4_eq(mat4_mul(m3, m2), c);

    c = (mat4){{
        15, 25, 26, 23,
        26, 44, 44, 42,
        30, 48, 59, 50,
        32, 52, 56, 74
    }};
    mat4_eq(mat4_mul(m2, m3), c);

    return 0;
}
int test_matrix_projection() {
    projection_settings s1 = {
        .dims = { .x = -100, .y = 0, .z = 100, .w = 0.1 },
        .is_ortho = true,
        .fov = 90
    };
    projection_settings s2 = {
        .dims = { .x = -100, .y = 10, .z = 100, .w = 0.1 },
        .is_ortho = false,
        .fov = 0
    };

    mat4 b = mat4_projection(s1);
    mat4 c = mat4_ident;
    mat4_eq(b, c);

    b = mat4_projection(s2);
    mat4_eq(b, c);

    return 0;
}
