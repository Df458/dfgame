#ifndef DF_MATH_TRANSFORM
#define DF_MATH_TRANSFORM

#include "matrix.hd"
#include "quat.h"
#include "transform.hd"
#include "vector.hd"

// Creates a new transform with default values
transform transform_new();

// Sets or adds the transform's position
#define transform_translate(t, v, r) _Generic(v,\
    vec2: transform_translate_2d,\
    vec3: transform_translate_3d\
)(t, v, r)
void transform_translate_2d(transform t, vec2 v, bool relative);
void transform_translate_3d(transform t, vec3 v, bool relative);

// Gets the position of the transform
vec3 transform_get_position(transform t);

// Rotates or sets the orientation of the transform
#define transform_rotate(t, v, r) _Generic(v,\
    double: transform_rotate_2d,\
    float: transform_rotate_2d,\
    vec3:  transform_rotate_3d,\
    quat:  transform_rotate_3d_quat\
)(t, v, r)
void transform_rotate_2d(transform t, float theta, bool relative);
void transform_rotate_3d(transform t, vec3 euler, bool relative);
void transform_rotate_3d_quat(transform t, quat q, bool relative);

// Sets the orientation of t to face target
void transform_lookat(transform t, vec3 target, vec3 up);

// Gets the orientation of the transform
quat transform_get_orientation(transform t);
vec3 transform_get_orientation_euler(transform t);
float transform_get_orientation_2d(transform t);

#define transform_scale(t, v, r) _Generic(v,\
    int:    transform_scale_1d,\
    double: transform_scale_1d,\
    float: transform_scale_1d,\
    vec2:  transform_scale_2d,\
    vec3:  transform_scale_3d\
)(t, v, r)
// Sets or adds the transform's scale
void transform_scale_1d(transform t, float v, bool relative);
void transform_scale_2d(transform t, vec2 v, bool relative);
void transform_scale_3d(transform t, vec3 v, bool relative);

// Gets the scale of the transform
vec3 transform_get_scale(transform t);

// Gets the matrix composed of the transform's attributes.
mat4 transform_get_matrix(transform t);

// Frees the transform
#define transform_free(t) { _transform_free(t); t = NULL; }
void _transform_free(transform t);

#endif
