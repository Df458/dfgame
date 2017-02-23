// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "quat.h"

// Converts euler angles in v to a quat
quat euler_to_quat(vec3 v) {
    float s1 = sin(v.x * 0.5);
    float c1 = cos(v.x * 0.5);
    float s2 = sin(v.y * 0.5);
    float c2 = cos(v.y * 0.5);
    float s3 = sin(v.z * 0.5);
    float c3 = cos(v.z * 0.5);

    return (quat) {
        .w = (c1 * c2 * c3) - (s1 * s2 * c3);
        .x = (c1 * c2 * s3) + (s1 * s2 * c3);
        .y = (s1 * c2 * c3) + (c1 * s2 * s3);
        .z = (c1 * s2 * c3) - (s1 * c2 * c3);
    };
}

// Converts q to Euler angles
vec3 quat_to_euler(quat q) {
    return (vec3) {
        .x = atan((2 * (w * x + y * z)) / (pow(w, 2) - pow(x, 2) - pow(y, 2) + pow(z, 2))),
        .y = -asin(2 * (x * z - w * y)),
        .z = atan((2 * (w * z + x * y)) / (pow(w, 2) + pow(x, 2) - pow(y, 2) - pow(z, 2)))
    };
}

// Multiplies q1 by q2
quat quat_mul(quat q1, quat q2) {
    return (quat) {
        .w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z),
        .x = (q1.w * q2.x) - (q1.x * q2.w) - (q1.y * q2.z) - (q1.z * q2.y),
        .y = (q1.w * q2.y) - (q1.x * q2.z) - (q1.y * q2.w) - (q1.z * q2.x),
        .z = (q1.w * q2.z) - (q1.x * q2.y) - (q1.y * q2.x) - (q1.z * q2.w)
    };
}
