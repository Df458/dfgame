#include "alignment.h"
#include "math/alignment.h"

#include <stdlib.h>
#include <time.h>

int test_alignment_init() {
    srand(time(NULL));

    return 0;
}
int test_alignment_cleanup() {
    return 0;
}
void test_alignment_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_alignment_origin_2d);
    CU_ADD_TEST(suite, test_alignment_origin_3d);
    CU_ADD_TEST(suite, test_alignment_point_2d);
    CU_ADD_TEST(suite, test_alignment_point_3d);
    CU_ADD_TEST(suite, test_alignment_box_2d);
    CU_ADD_TEST(suite, test_alignment_box_3d);
}

void test_alignment_origin_2d() {
    aabb_2d zero_box = aabb_2d_zero;
    aabb_2d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = vec2_zero
    };
    aabb_2d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100, .y = rand() % 100 },
    };

    for(alignment_2d a = 0; a <= ALIGN_LAST; ++a) {
        vec2 result = aabb_get_origin_2d(zero_box, a);
        CU_ASSERT(eq0(result.x));
        CU_ASSERT(eq0(result.y));

        result = aabb_get_origin_2d(box1, a);
        CU_ASSERT(eq0(result.x - box1.position.x));
        CU_ASSERT(eq0(result.y - box1.position.y));
    }

    vec2 result;

    result = aabb_get_origin_2d(box2, ALIGN_TOP);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y));

    result = aabb_get_origin_2d(box2, ALIGN_BOTTOM);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));

    result = aabb_get_origin_2d(box2, ALIGN_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    result = aabb_get_origin_2d(box2, ALIGN_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));

    result = aabb_get_origin_2d(box2, ALIGN_TOP_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    result = aabb_get_origin_2d(box2, ALIGN_TOP_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    result = aabb_get_origin_2d(box2, ALIGN_BOTTOM_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    result = aabb_get_origin_2d(box2, ALIGN_BOTTOM_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));

    result = aabb_get_origin_2d(box2, ALIGN_CENTER);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
}
void test_alignment_origin_3d() {
    aabb_3d zero_box = aabb_3d_zero;
    aabb_3d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50, .z = rand() % 100 - 50 },
        .dimensions = vec3_zero
    };
    aabb_3d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50, .z = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100, .y = rand() % 100, .z = rand() % 100}
    };

    for(alignment_3d a = 0; a <= ALIGN_3D_LAST; ++a) {
        vec3 result = aabb_get_origin_3d(zero_box, a);
        CU_ASSERT(eq0(result.x));
        CU_ASSERT(eq0(result.y));
        CU_ASSERT(eq0(result.z));

        result = aabb_get_origin_3d(box1, a);
        CU_ASSERT(eq0(result.x - box1.position.x));
        CU_ASSERT(eq0(result.y - box1.position.y));
        CU_ASSERT(eq0(result.z - box1.position.z));
    }

    vec3 result;

    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_LEFT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_LEFT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_RIGHT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_RIGHT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z));

    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_LEFT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_LEFT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_RIGHT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_RIGHT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_TOP_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y));
    CU_ASSERT(eq0(result.z - box2.position.z));

    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_LEFT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_LEFT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_LEFT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_RIGHT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z) - (box2.dimensions.z * 0.5f));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_RIGHT_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_RIGHT_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - box2.dimensions.x));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_FRONT);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z - box2.dimensions.z));
    result = aabb_get_origin_3d(box2, ALIGN_3D_BOTTOM_BACK);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - box2.dimensions.y));
    CU_ASSERT(eq0(result.z - box2.position.z));

    result = aabb_get_origin_3d(box2, ALIGN_3D_CENTER);
    CU_ASSERT(eq0(result.x - box2.position.x - (box2.dimensions.x * 0.5f)));
    CU_ASSERT(eq0(result.y - box2.position.y - (box2.dimensions.y * 0.5f)));
    CU_ASSERT(eq0(result.z - box2.position.z - (box2.dimensions.z * 0.5f)));
}
void test_alignment_point_2d() {
    vec2 point = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 };
    aabb_2d zero_box = aabb_2d_zero;
    aabb_2d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = vec2_zero
    };
    aabb_2d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100, .y = rand() % 100 },
    };
    float x_aligns[] = {
        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,
    };
    float y_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,
    };

    for(alignment_2d a = 0; a <= ALIGN_LAST; ++a) {
        aabb_2d result = aabb_align_point_2d(zero_box, point, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));

        result = aabb_align_point_2d(box1, point, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));

        result = aabb_align_point_2d(box2, point, a);
        CU_ASSERT(eq0(result.position.x - point.x + (box2.dimensions.x * x_aligns[a])));
        CU_ASSERT(eq0(result.position.y - point.y + (box2.dimensions.y * y_aligns[a])));
    }
}
void test_alignment_point_3d() {
    vec3 point = { .x = rand() % 100 - 50, .y = rand() % 100 - 50, .z = rand() % 100 - 50 };
    aabb_3d zero_box = aabb_3d_zero;
    aabb_3d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100, .z = rand() % 100 },
        .dimensions = vec3_zero
    };
    aabb_3d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50, .z = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100 - 50, .y = rand() % 100, .z = rand() % 100 },
    };
    float x_aligns[] = {
        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,
    };
    float y_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        0.5f,
        0.5f,
        0.5f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        1.0f,
        1.0f,
        1.0f,

        1.0f,
        1.0f,
        1.0f,
    };
    float z_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,
    };

    for(alignment_3d a = 0; a <= ALIGN_3D_LAST; ++a) {
        aabb_3d result = aabb_align_point_3d(zero_box, point, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));
        CU_ASSERT(eq0(result.position.z - point.z));

        result = aabb_align_point_3d(box1, point, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));
        CU_ASSERT(eq0(result.position.z - point.z));

        result = aabb_align_point_3d(box2, point, a);
        CU_ASSERT(eq0(result.position.x - point.x + (box2.dimensions.x * x_aligns[a])));
        CU_ASSERT(eq0(result.position.y - point.y + (box2.dimensions.y * y_aligns[a])));
        CU_ASSERT(eq0(result.position.z - point.z + (box2.dimensions.z * z_aligns[a])));
    }
}
void test_alignment_box_2d() {
    aabb_2d zero_box = aabb_2d_zero;
    aabb_2d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100, .y = rand() % 100 },
    };
    aabb_2d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = { .x = box1.dimensions.x + (rand() % 100), .y = box1.dimensions.y + (rand() % 100) },
    };
    float x_aligns[] = {
        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,
    };
    float y_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,
    };

    for(alignment_2d a = 0; a <= ALIGN_LAST; ++a) {
        aabb_2d result = aabb_align_box_2d(box1, zero_box, a);
        CU_ASSERT(eq0(result.position.x));
        CU_ASSERT(eq0(result.position.y));
        CU_ASSERT(eq0(result.dimensions.x));
        CU_ASSERT(eq0(result.dimensions.y));

        result = aabb_align_box_2d(zero_box, box2, a);
        vec2 point = aabb_get_origin_2d(box2, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));

        result = aabb_align_box_2d(box1, box2, a);
        CU_ASSERT(eq0(result.position.x - point.x + (box1.dimensions.x * x_aligns[a])));
        CU_ASSERT(eq0(result.position.y - point.y + (box1.dimensions.y * y_aligns[a])));
    }
}
void test_alignment_box_3d() {
    aabb_3d zero_box = aabb_3d_zero;
    aabb_3d box1 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50, .z = rand() % 100 - 50 },
        .dimensions = { .x = rand() % 100, .y = rand() % 100, .z = rand() % 100 },
    };
    aabb_3d box2 = {
        .position = { .x = rand() % 100 - 50, .y = rand() % 100 - 50 },
        .dimensions = { .x = box1.dimensions.x + (rand() % 100), .y = box1.dimensions.y + (rand() % 100), .z = box1.dimensions.z + (rand() % 100) },
    };
    float x_aligns[] = {
        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,

        0.0f,
        0.5f,
        1.0f,
    };
    float y_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        0.5f,
        0.5f,
        0.5f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        1.0f,
        1.0f,
        1.0f,

        1.0f,
        1.0f,
        1.0f,
    };
    float z_aligns[] = {
        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,

        0.0f,
        0.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,

        1.0f,
        1.0f,
        1.0f,
    };

    for(alignment_3d a = 0; a <= ALIGN_3D_LAST; ++a) {
        aabb_3d result = aabb_align_box_3d(box1, zero_box, a);
        CU_ASSERT(eq0(result.position.x));
        CU_ASSERT(eq0(result.position.y));
        CU_ASSERT(eq0(result.position.z));
        CU_ASSERT(eq0(result.dimensions.x));
        CU_ASSERT(eq0(result.dimensions.y));
        CU_ASSERT(eq0(result.dimensions.z));

        result = aabb_align_box_3d(zero_box, box2, a);
        vec3 point = aabb_get_origin_3d(box2, a);
        CU_ASSERT(eq0(result.position.x - point.x));
        CU_ASSERT(eq0(result.position.y - point.y));
        CU_ASSERT(eq0(result.position.z - point.z));

        result = aabb_align_box_3d(box1, box2, a);
        CU_ASSERT(eq0(result.position.x - point.x + (box1.dimensions.x * x_aligns[a])));
        CU_ASSERT(eq0(result.position.y - point.y + (box1.dimensions.y * y_aligns[a])));
        CU_ASSERT(eq0(result.position.z - point.z + (box1.dimensions.z * z_aligns[a])));
    }
}
