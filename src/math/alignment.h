#ifndef DF_ALIGNMENT_H
#define DF_ALIGNMENT_H
#include "math/aabb.h"
#include "math/vector.h"

typedef enum alignment_2d {
    ALIGN_TOP_LEFT = 0,
    ALIGN_TOP,
    ALIGN_TOP_RIGHT,

    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT,

    ALIGN_BOTTOM_LEFT,
    ALIGN_BOTTOM,
    ALIGN_BOTTOM_RIGHT,

    ALIGN_LAST    = ALIGN_BOTTOM_RIGHT,
    ALIGN_DEFAULT = ALIGN_TOP_LEFT
} alignment_2d;
typedef enum alignment_3d {
    ALIGN_3D_TOP_LEFT_BACK = 0,
    ALIGN_3D_TOP_BACK,
    ALIGN_3D_TOP_RIGHT_BACK,

    ALIGN_3D_TOP_LEFT,
    ALIGN_3D_TOP,
    ALIGN_3D_TOP_RIGHT,

    ALIGN_3D_TOP_LEFT_FRONT,
    ALIGN_3D_TOP_FRONT,
    ALIGN_3D_TOP_RIGHT_FRONT,

    ALIGN_3D_LEFT_BACK,
    ALIGN_3D_BACK,
    ALIGN_3D_RIGHT_BACK,

    ALIGN_3D_LEFT,
    ALIGN_3D_CENTER,
    ALIGN_3D_RIGHT,

    ALIGN_3D_LEFT_FRONT,
    ALIGN_3D_FRONT,
    ALIGN_3D_RIGHT_FRONT,

    ALIGN_3D_BOTTOM_LEFT_BACK,
    ALIGN_3D_BOTTOM_BACK,
    ALIGN_3D_BOTTOM_RIGHT_BACK,
    ALIGN_3D_BOTTOM_LEFT,
    ALIGN_3D_BOTTOM,
    ALIGN_3D_BOTTOM_RIGHT,
    ALIGN_3D_BOTTOM_LEFT_FRONT,
    ALIGN_3D_BOTTOM_FRONT,
    ALIGN_3D_BOTTOM_RIGHT_FRONT,

    ALIGN_3D_LAST    = ALIGN_3D_BOTTOM_RIGHT_FRONT,
    ALIGN_3D_DEFAULT = ALIGN_3D_TOP_LEFT_BACK
} alignment_3d;

// Get a point aligned with the given aabb
#define aabb_get_origin(a, b) _Generic(a,\
    aabb_2d: aabb_get_origin_2d,\
    aabb_3d: aabb_get_origin_3d\
)(a, b)
vec2 aabb_get_origin_2d(aabb_2d box, alignment_2d align);
vec3 aabb_get_origin_3d(aabb_3d box, alignment_3d align);

// Get an aabb with the dimensions of box2 aligned to a point
#define aabb_align_point(a, b, c) _Generic(a,\
    aabb_2d: aabb_align_point_2d,\
    aabb_3d: aabb_align_point_3d\
)(a, b, c)
aabb_2d aabb_align_point_2d(aabb_2d box, vec2 point, alignment_2d align);
aabb_3d aabb_align_point_3d(aabb_3d box, vec3 point, alignment_3d align);

// Constrain a vector to the given box
#define aabb_constrain_point(a, b) _Generic(a,\
    aabb_2d: aabb_constrain_point_2d,\
    aabb_3d: aabb_constrain_point_3d\
)(a, b)
vec2 aabb_constrain_point_2d(aabb_2d box, vec2 point);
vec3 aabb_constrain_point_3d(aabb_3d box, vec3 point);

// Get an aabb with the dimensions of box aligned to box2
#define aabb_align_box(a, b, c) _Generic(a,\
    aabb_2d: aabb_align_box_2d,\
    aabb_3d: aabb_align_box_3d\
)(a, b, c)
aabb_2d aabb_align_box_2d(aabb_2d box, aabb_2d box2, alignment_2d align);
aabb_3d aabb_align_box_3d(aabb_3d box, aabb_3d box2, alignment_3d align);

#endif
