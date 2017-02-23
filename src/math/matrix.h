#ifndef DF_MATH_MATRIX
#define DF_MATH_MATRIX

#include "quat.hd"
#include "vector.hd"

// Represents a 4x4 transformation matrix
typedef struct mat4 {
    float data[16];
} mat4;

#define mat4_ident (mat4){ {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} }

// Translates the transformation matrix m by v
#define mat4_translate(m, v) _Generic(v,\
    vec2: mat4_translate_vec2(m, v)\
    vec3: mat4_translate_vec3(m, v)\
    vec4: mat4_translate_vec4(m, v)\
)
mat4 mat4_translate_vec2(mat4 m, vec2 v);
mat4 mat4_translate_vec3(mat4 m, vec3 v);
mat4 mat4_translate_vec4(mat4 m, vec4 v);

// Rotates the transformation matrix m by r (in radians)
#define mat4_rotate(m, r) _Generic(r,\
    float: mat4_rotate_2d(m, r)\
    vec3:  mat4_rotate_3d(m, r)\
    quat:  mat4_rotate_3d_quat(m, r)\
)

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
    vec2: mat4_scale_vec2(m, v)\
    vec3: mat4_scale_vec3(m, v)\
    vec4: mat4_scale_vec4(m, v)\
)
mat4 mat4_scale_vec2(mat4 m, vec2 v);
mat4 mat4_scale_vec3(mat4 m, vec3 v);
mat4 mat4_scale_vec4(mat4 m, vec4 v);

// Multiplies m1 by m2
mat4 mat4_mul(mat4 m1, mat4 m2);

// Gets the determinant of m
float mat4_determinant(mat4 m);

// Transposes m
mat4 mat4_transpose(mat4 m);

#endif
