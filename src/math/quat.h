#ifndef DF_MATH_QUAT
#define DF_MATH_QUAT

#include "vector.h"

// Represents a quaternion rotation
typedef struct quat {
    union {
        struct {
            float w;
            float x;
            float y;
            float z;
        };
        float data[4];
    };
} quat;

#define quat_ident (quat){ .w = 1, .x = 0, .y = 0, .z = 0};
#define make_quat(axis, angle) (quat){ .w = cos(0.5 * angle), .x = axis.x * sin(0.5 * angle), .y = axis.y * sin(0.5 * angle), .z = axis.z * sin(0.5 * angle) }

// Converts euler angles in v to a quat
quat euler_to_quat(vec3 v);
#define euler_to_quat_deg(v) euler_to_quat((vec3){.x=degtorad(v.x),.y=degtorad(v.y),.z=degtorad(v.z)});

// Converts q to Euler angles
vec3 quat_to_euler(quat q);

#define quat_mul(q, v) _Generic(v,\
    double: quat_mul_float,\
    float: quat_mul_float,\
    quat: quat_mul_quat\
)(q, v)
// Multiplies q1 by q2
quat quat_mul_float(quat q1, float f);
quat quat_mul_quat(quat q1, quat q2);

// Normalizes a quaternion
quat quat_normalize(quat q);

// printf helper macros
#define quat_printstr "[%f %f %f %f]"
#define quat_printargs(q) q.data[0], q.data[1], q.data[2], q.data[3]

#endif
