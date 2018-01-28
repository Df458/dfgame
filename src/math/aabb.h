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
typedef struct aabb_3d {
    vec3 position;
    vec3 dimensions;
} aabb_3d;

// Gets the area of the aabb
#define aabb_area(box) _Generic(box,\
    aabb_2d: aabb_area_2d,\
    aabb_3d: aabb_area_3d,\
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

#endif
