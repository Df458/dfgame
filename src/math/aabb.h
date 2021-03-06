#ifndef DF_MATH_AABB
#define DF_MATH_AABB

#include "vector.h"

// Represents a bounding box
typedef struct aabb_2d {
    union {
        struct {
            vec2 position;
            vec2 dimensions;
        };
        struct {
            float x;
            float y;
            float width;
            float height;
        };
        vec4 vec;
    };
} aabb_2d;

// A bounding box at [0,0] with dimensions of [0,0]
const extern aabb_2d aabb_2d_zero;

typedef struct aabb_3d {
    union {
        struct {
            vec3 position;
            vec3 dimensions;
        };
        struct {
            float x;
            float y;
            float z;
            float width;
            float height;
            float depth;
        };
    };
} aabb_3d;

// A bounding box at [0,0,0] with dimensions of [0,0,0]
const extern aabb_3d aabb_3d_zero;

// Gets the area of the aabb
#define aabb_area(box) _Generic(box,\
    aabb_2d: aabb_area_2d,\
    aabb_3d: aabb_volume_3d\
)(box)
float aabb_area_2d(aabb_2d box);
float aabb_volume_3d(aabb_3d box);

// Gets whether or not the two aabbs intersect
#define aabb_intersects(box, v) _Generic(box,\
    aabb_2d: aabb_intersects_2d,\
    aabb_3d: aabb_intersects_3d\
)(box, v)
#define aabb_intersects_point(box, v) _Generic(box,\
    aabb_2d: aabb_intersects_point_2d,\
    aabb_3d: aabb_intersects_point_3d\
)(box, v)
bool aabb_intersects_2d(aabb_2d box1, aabb_2d box2);
bool aabb_intersects_3d(aabb_3d box1, aabb_3d box2);
bool aabb_intersects_point_2d(aabb_2d box, vec2 point);
bool aabb_intersects_point_3d(aabb_3d box, vec3 point);

// Returns a new aabb based on the overlap of the two given aabbs
#define aabb_get_intersection(a, b) _Generic(box,\
    aabb_2d: aabb_get_intersection_2d,\
    aabb_3d: aabb_get_intersection_3d,\
)(a, b)
aabb_2d aabb_get_intersection_2d(aabb_2d box1, aabb_2d box2);
aabb_3d aabb_get_intersection_3d(aabb_3d box1, aabb_3d box2);

#define aabb_comparison(a, b) _Generic(a,\
    aabb_2d: aabb_comparison_2d,\
    aabb_3d: aabb_comparison_3d,\
)(a, b)
comparison aabb_comparison_2d(aabb_2d box1, aabb_2d box2);
comparison aabb_comparison_3d(aabb_3d box1, aabb_3d box2);

// printf helper macros
#define aabb_2d_printstr "[(%f, %f), %fx%f]"
#define aabb_3d_printstr "[(%f, %f, %f), %fx%fx%f]"
#define aabb_2d_decomp(b) vec2_decomp(b.position), vec2_decomp(b.dimensions)
#define aabb_3d_decomp(b) vec3_decomp(b.position), vec3_decomp(b.dimensions)

#endif
