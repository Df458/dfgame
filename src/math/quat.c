// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "quat.h"

#include <math.h>

// Converts euler angles in v to a quat
quat euler_to_quat(vec3 v) {
    float s3 = sin(v.x * 0.5);
    float c3 = cos(v.x * 0.5);
    float s2 = sin(v.z * 0.5);
    float c2 = cos(v.z * 0.5);
    float s1 = sin(v.y * 0.5);
    float c1 = cos(v.y * 0.5);

    return (quat) {
        .w = (c1 * c2 * c3) - (s1 * s2 * s3),
        .x = (s1 * s2 * c3) + (c1 * c2 * s3),
        .y = (s1 * c2 * c3) + (c1 * s2 * s3),
        .z = (c1 * s2 * c3) - (s1 * c2 * s3)
    };
}

// Converts q to Euler angles
vec3 quat_to_euler(quat q) {
    return (vec3) {
        .x = atan2((2 * (q.w * q.x + q.y * q.z)), 1 - 2 * (square(q.x) + square(q.y))),
        .y = -asin(2 * (q.x * q.z - q.w * q.y)),
        .z = atan2((2 * (q.w * q.z + q.x * q.y)), 1 - 2 * (square(q.y) + square(q.z)))
    };
}

// Multiplies q1 by q2
// NOTE: Copied from [http://www.gamasutra.com/view/feature/131686/rotating_objects_using_quaternions.php]
quat quat_mul_quat(quat q1, quat q2) {
    float A, B, C, D, E, F, G, H;
    A = (q1.w + q1.x)*(q2.w + q2.x);
    B = (q1.z - q1.y)*(q2.y - q2.z);
    C = (q1.w - q1.x)*(q2.y + q2.z); 
    D = (q1.y + q1.z)*(q2.w - q2.x);
    E = (q1.x + q1.z)*(q2.x + q2.y);
    F = (q1.x - q1.z)*(q2.x - q2.y);
    G = (q1.w + q1.y)*(q2.w - q2.z);
    H = (q1.w - q1.y)*(q2.w + q2.z);
    return (quat) {
        .w = B + (-E - F + G + H)/2,
        .x = A - ( E + F + G + H)/2,
        .y = C + ( E - F + G - H)/2,
        .z = D + ( E - F - G + H)/2
    };
}
quat quat_mul_float(quat q1, float f) {
    return (quat) {
        .w = q1.w * f,
        .x = q1.x * f,
        .y = q1.y * f,
        .z = q1.z * f
    };
}

// Normalizes a quaternion
quat quat_normalize(quat q) {
    float norm = sqrt(square(q.w) + square(q.x) + square(q.y) + square(q.z));
    return (quat) {
        .w = q.w / norm,
        .x = q.x / norm,
        .y = q.y / norm,
        .z = q.z / norm
    };
}
