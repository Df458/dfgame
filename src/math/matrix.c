// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "matrix.h"

#include "quat.h"
#include "types.h"
#include "vector.h"
#include <math.h>

// Translates the transformation matrix m by v
mat4 mat4_translate_vec2(mat4 m, vec2 v) {
    mat4 m2 = m;
    m2.data[12] += v.x;
    m2.data[13] += v.y;

    return m2;
}
mat4 mat4_translate_vec3(mat4 m, vec3 v) {
    mat4 m2 = m;
    m2.data[12] += v.x;
    m2.data[13] += v.y;
    m2.data[14] += v.z;

    return m2;
}
mat4 mat4_translate_vec4(mat4 m, vec4 v) {
    mat4 m2 = m;
    m2.data[12] += v.x;
    m2.data[13] += v.y;
    m2.data[14] += v.z;
    m2.data[15] += v.w;

    return m2;
}

// Rotates the transformation matrix m by r
mat4 mat4_rotate_2d(mat4 m, float theta) {
    mat4 m2 = m;
    float s = sin(theta);
    float c = cos(theta);

    m2.data[0] +=  c;
    m2.data[1] +=  s;
    m2.data[4] += -s;
    m2.data[5] +=  c;

    return m2;
}
mat4 mat4_rotate_3d(mat4 m, vec3 euler) {
    mat4 m2 = m;
    float sx = sin(euler.x);
    float cx = cos(euler.x);
    float sy = sin(euler.y);
    float cy = cos(euler.y);
    float sz = sin(euler.x);
    float cz = cos(euler.x);

    m2.data[0]  += 1 + cy + cz;
    m2.data[1]  += sz;
    m2.data[3]  += sy;
    m2.data[4]  -= sz;
    m2.data[5]  += 1 + cx + cz;
    m2.data[6]  += sx;
    m2.data[8]  -= sy;
    m2.data[9]  -= sx;
    m2.data[10] += 1 + cx + cy;

    return m2;
}
mat4 mat4_rotate_3d_quat(mat4 m, quat q) {
    mat4 m2 = m;

    m2.data[0]  = pow(q.w, 2) + pow(q.x, 2) - pow(q.y, 2) - pow(q.z, 2);
    m2.data[1]  = (2 * q.x * q.y) - (2 * q.w * q.z);
    m2.data[2]  = (2 * q.x * q.z) + (2 * q.w * q.y);
    m2.data[4]  = (2 * q.x * q.y) + (2 * q.w * q.z);
    m2.data[5]  = pow(q.w, 2) - pow(q.x, 2) + pow(q.y, 2) - pow(q.z, 2);
    m2.data[6]  = (2 * q.y * q.z) + (2 * q.w * q.x);
    m2.data[8]  = (2 * q.x * q.z) - (2 * q.w * q.y);
    m2.data[9]  = (2 * q.y * q.z) - (2 * q.w * q.x);
    m2.data[10] = pow(q.w, 2) - pow(q.x, 2) - pow(q.y, 2) + pow(q.z, 2);

    return m2;
}

// Rotates the transformation matrix m by v
mat4 mat4_scale_vec2(mat4 m, vec2 v) {
    mat4 m2 = m;
    m2.data[0] *= v.x;
    m2.data[5] *= v.y;

    return m2;
}
mat4 mat4_scale_vec3(mat4 m, vec3 v) {
    mat4 m2 = m;
    m2.data[0]  *= v.x;
    m2.data[5]  *= v.y;
    m2.data[10] *= v.z;

    return m2;
}
mat4 mat4_scale_vec4(mat4 m, vec4 v) {
    mat4 m2 = m;
    m2.data[0]  *= v.x;
    m2.data[5]  *= v.y;
    m2.data[10] *= v.z;
    m2.data[15] *= v.w;

    return m2;
}

// Multiplies m1 by m2
mat4 mat4_mul(mat4 m1, mat4 m2) {
    mat4 res;
    for(uint8 i = 0; i < 4; ++i) {
        uint8 j = i * 4;
        res.data[j    ] = m1.data[0]*m2.data[j] + m1.data[4]*m2.data[j+1] + m1.data[8] *m2.data[j+2]  + m1.data[12]*m2.data[j+3];
        res.data[j + 1] = m1.data[1]*m2.data[j] + m1.data[5]*m2.data[j+1] + m1.data[9] *m2.data[j+2]  + m1.data[13]*m2.data[j+3];
        res.data[j + 2] = m1.data[2]*m2.data[j] + m1.data[6]*m2.data[j+1] + m1.data[10]*m2.data[j+2] + m1.data[14]*m2.data[j+3];
        res.data[j + 3] = m1.data[3]*m2.data[j] + m1.data[7]*m2.data[j+1] + m1.data[11]*m2.data[j+2] + m1.data[15]*m2.data[j+3];
    }
    return res;
}

// Gets the determinant of m
float mat4_determinant(mat4 m) {
    return m.data[12] * m.data[9] * m.data[6]  * m.data[3]  - m.data[8] * m.data[13] * m.data[6]  * m.data[3]  -
           m.data[12] * m.data[5] * m.data[10] * m.data[3]  + m.data[4] * m.data[13] * m.data[10] * m.data[3]  +
           m.data[8]  * m.data[5] * m.data[14] * m.data[3]  - m.data[4] * m.data[9]  * m.data[14] * m.data[3]  -
           m.data[12] * m.data[9] * m.data[2]  * m.data[7]  + m.data[8] * m.data[13] * m.data[2]  * m.data[7]  +
           m.data[12] * m.data[1] * m.data[10] * m.data[7]  - m.data[0] * m.data[13] * m.data[10] * m.data[7]  -
           m.data[8]  * m.data[1] * m.data[14] * m.data[7]  + m.data[0] * m.data[9]  * m.data[14] * m.data[7]  +
           m.data[12] * m.data[5] * m.data[2]  * m.data[11] - m.data[4] * m.data[13] * m.data[2]  * m.data[11] -
           m.data[12] * m.data[1] * m.data[6]  * m.data[11] + m.data[0] * m.data[13] * m.data[6]  * m.data[11] +
           m.data[4]  * m.data[1] * m.data[14] * m.data[11] - m.data[0] * m.data[5]  * m.data[14] * m.data[11] -
           m.data[8]  * m.data[5] * m.data[2]  * m.data[15] + m.data[4] * m.data[9]  * m.data[2]  * m.data[15] +
           m.data[8]  * m.data[1] * m.data[6]  * m.data[15] - m.data[0] * m.data[9]  * m.data[6]  * m.data[15] -
           m.data[4]  * m.data[1] * m.data[10] * m.data[15] + m.data[0] * m.data[5]  * m.data[10] * m.data[15];
}

// Transposes m
mat4 mat4_transpose(mat4 m) {
    mat4 m2 = mat4_ident;
    for(int i = 0; i < 3; ++i) {
        for(int j = i + 1; j < 4; ++j) {
            m2.data[i * 4 + j] = m.data[j * 4 + i];
            m2.data[j * 4 + i] = m.data[i * 4 + j];
        }
    }

    return m2;
}
