#ifndef DF_MATH_MATRIX
#define DF_MATH_MATRIX

#include "mathutil.h"
#include "quat.h"
#include "vector.hd"

// Represents a 4x4 transformation matrix
typedef struct mat4 {
    float data[16];
} mat4;

// Settings for creating a projection matrix
typedef struct projection_settings {
    vec4 dims;
    bool is_ortho;
    float fov;
} projection_settings;

#define mat4_ident ((mat4){ {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} })

// Translates the transformation matrix m by v
#define mat4_translate(m, v) _Generic(v,\
    vec2: mat4_translate_vec2,\
    vec3: mat4_translate_vec3,\
    vec4: mat4_translate_vec4\
)(m, v)
mat4 mat4_translate_vec2(mat4 m, vec2 v);
mat4 mat4_translate_vec3(mat4 m, vec3 v);
mat4 mat4_translate_vec4(mat4 m, vec4 v);

// Rotates the transformation matrix m by r (in radians)
#define mat4_rotate(m, r) _Generic(r,\
    float: mat4_rotate_2d,\
    vec3:  mat4_rotate_3d,\
    quat:  mat4_rotate_3d_quat\
)(m, r)

// Rotates the transformation matrix m by r (in degrees)
#define mat4_rotate_deg(m, r) _Generic(r,\
    float: mat4_rotate_2d(m, degtorad(r))\
    vec3:  mat4_rotate_3d(m, (vec3){.x=degtorad(r.x),.y=degtorad(r.y),.z=degtorad(r.z)})\
)
mat4 mat4_rotate_2d(mat4 m, float theta);
mat4 mat4_rotate_3d(mat4 m, vec3 euler);
mat4 mat4_rotate_3d_quat(mat4 m, quat q);

// Rotates the transformation matrix m by v
#define mat4_scale(m, v) _Generic(v,\
    double: mat4_scale_f,\
    float: mat4_scale_f,\
    int: mat4_scale_f,\
    vec2: mat4_scale_vec2,\
    vec3: mat4_scale_vec3,\
    vec4: mat4_scale_vec4\
)(m, v)
mat4 mat4_scale_f(mat4 m, float v);
mat4 mat4_scale_vec2(mat4 m, vec2 v);
mat4 mat4_scale_vec3(mat4 m, vec3 v);
mat4 mat4_scale_vec4(mat4 m, vec4 v);

// Multiplies m1 by m2
mat4 mat4_mul(mat4 m1, mat4 m2);

// Gets the determinant of m
float mat4_determinant(mat4 m);

// Transposes m
mat4 mat4_transpose(mat4 m);

// Inverts m
mat4 mat4_invert(mat4 m);

// Creates a new projection matrix using the provided settings
mat4 mat4_projection(projection_settings settings);

// printf helper macros
#define mat4_printstr "[%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f]"
#define mat4_printargs(m) m.data[0], m.data[1], m.data[2], m.data[3], m.data[4], m.data[5], m.data[6], m.data[7], m.data[8], m.data[9], m.data[10], m.data[11], m.data[12], m.data[13], m.data[14], m.data[15], m.data[16]

#endif
