// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "matrix.h"

#include "log/log.h"
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
    mat4 m2 = mat4_ident;
    float s = sin(theta);
    float c = cos(theta);

    m2.data[0] =  c;
    m2.data[1] =  s;
    m2.data[4] = -s;
    m2.data[5] =  c;

    return mat4_mul(m, m2);
}
mat4 mat4_rotate_3d(mat4 m, vec3 euler) {
    float sx = sin(euler.x);
    float cx = cos(euler.x);
    float sy = sin(euler.y);
    float cy = cos(euler.y);
    float sz = sin(euler.z);
    float cz = cos(euler.z);

    mat4 mx = mat4_ident;
    mat4 my = mat4_ident;
    mat4 mz = mat4_ident;

    mx.data[5] = cx;
    mx.data[6] = -sx;
    mx.data[9] = sx;
    mx.data[10] = cx;

    my.data[0] = cy;
    my.data[2] = sy;
    my.data[4] = -sy;
    my.data[6] = cy;

    mz.data[0] = cz;
    mz.data[1] = -sz;
    mz.data[4] = sz;
    mz.data[5] = cz;

    return mat4_mul(m, mat4_mul(mx, mat4_mul(my, mz)));
}
mat4 mat4_rotate_3d_quat(mat4 m, quat q) {
    mat4 m2 = m;

    m2.data[0]  -= (2 * square(q.y)) + (2 * square(q.z));
    m2.data[1]  += (2 * q.x * q.y)   - (2 * q.w * q.z);
    m2.data[2]  += (2 * q.x * q.z)   + (2 * q.w * q.y);
    m2.data[4]  += (2 * q.x * q.y)   + (2 * q.w * q.z);
    m2.data[5]  -= (2 * square(q.x)) + (2 * square(q.z));
    m2.data[6]  += (2 * q.y * q.z)   - (2 * q.w * q.x);
    m2.data[8]  += (2 * q.x * q.z)   - (2 * q.w * q.y);
    m2.data[9]  += (2 * q.y * q.z)   + (2 * q.w * q.x);
    m2.data[10] -= (2 * square(q.x)) + (2 * square(q.y));

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
        res.data[j    ] = m1.data[0]*m2.data[j] + m1.data[4]*m2.data[j+1] + m1.data[8] *m2.data[j+2] + m1.data[12]*m2.data[j+3];
        res.data[j + 1] = m1.data[1]*m2.data[j] + m1.data[5]*m2.data[j+1] + m1.data[9] *m2.data[j+2] + m1.data[13]*m2.data[j+3];
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

// Inverts m
mat4 mat4_invert(mat4 m) {
    float det = mat4_determinant(m);

    check_return(det != 0, "Cannot invert matrix: Matrix is non-invertible", mat4_ident);

    float invdet = 1.0f / det;

    return (mat4) {.data={
        ((m.data[ 5]*m.data[10]*m.data[15]) + (m.data[ 6]*m.data[11]*m.data[13]) + (m.data[ 7]*m.data[ 9]*m.data[14]) - (m.data[ 5]*m.data[11]*m.data[14]) - (m.data[ 6]*m.data[ 9]*m.data[15]) - (m.data[ 7]*m.data[10]*m.data[13])) * invdet,
        ((m.data[ 1]*m.data[11]*m.data[14]) + (m.data[ 2]*m.data[ 9]*m.data[15]) + (m.data[ 3]*m.data[10]*m.data[13]) - (m.data[ 1]*m.data[10]*m.data[15]) - (m.data[ 2]*m.data[11]*m.data[13]) - (m.data[ 3]*m.data[ 9]*m.data[14])) * invdet,
        ((m.data[ 1]*m.data[ 6]*m.data[15]) + (m.data[ 2]*m.data[ 7]*m.data[13]) + (m.data[ 3]*m.data[ 5]*m.data[14]) - (m.data[ 1]*m.data[ 7]*m.data[14]) - (m.data[ 2]*m.data[ 5]*m.data[15]) - (m.data[ 3]*m.data[ 6]*m.data[13])) * invdet,
        ((m.data[ 1]*m.data[ 7]*m.data[10]) + (m.data[ 2]*m.data[ 5]*m.data[11]) + (m.data[ 3]*m.data[ 6]*m.data[ 9]) - (m.data[ 1]*m.data[ 6]*m.data[11]) - (m.data[ 2]*m.data[ 7]*m.data[ 9]) - (m.data[ 3]*m.data[ 5]*m.data[10])) * invdet,

        ((m.data[ 4]*m.data[11]*m.data[14]) + (m.data[ 6]*m.data[ 8]*m.data[15]) + (m.data[ 7]*m.data[10]*m.data[12]) - (m.data[ 4]*m.data[10]*m.data[15]) - (m.data[ 6]*m.data[11]*m.data[12]) - (m.data[ 7]*m.data[ 8]*m.data[14])) * invdet,
        ((m.data[ 0]*m.data[10]*m.data[15]) + (m.data[ 2]*m.data[11]*m.data[12]) + (m.data[ 3]*m.data[ 8]*m.data[14]) - (m.data[ 0]*m.data[11]*m.data[14]) - (m.data[ 2]*m.data[ 8]*m.data[15]) - (m.data[ 3]*m.data[10]*m.data[12])) * invdet,
        ((m.data[ 0]*m.data[ 7]*m.data[14]) + (m.data[ 2]*m.data[ 4]*m.data[15]) + (m.data[ 3]*m.data[ 6]*m.data[12]) - (m.data[ 0]*m.data[ 6]*m.data[15]) - (m.data[ 2]*m.data[ 7]*m.data[12]) - (m.data[ 3]*m.data[ 4]*m.data[14])) * invdet,
        ((m.data[ 0]*m.data[ 6]*m.data[11]) + (m.data[ 2]*m.data[ 7]*m.data[ 8]) + (m.data[ 3]*m.data[ 4]*m.data[10]) - (m.data[ 0]*m.data[ 7]*m.data[10]) - (m.data[ 2]*m.data[ 4]*m.data[11]) - (m.data[ 3]*m.data[ 6]*m.data[ 8])) * invdet,

        ((m.data[ 4]*m.data[ 9]*m.data[15]) + (m.data[ 5]*m.data[11]*m.data[12]) + (m.data[ 7]*m.data[ 8]*m.data[13]) - (m.data[ 4]*m.data[11]*m.data[13]) - (m.data[ 5]*m.data[ 8]*m.data[15]) - (m.data[ 7]*m.data[ 9]*m.data[12])) * invdet,
        ((m.data[ 0]*m.data[11]*m.data[13]) + (m.data[ 1]*m.data[ 8]*m.data[15]) + (m.data[ 3]*m.data[ 9]*m.data[12]) - (m.data[ 0]*m.data[ 9]*m.data[15]) - (m.data[ 1]*m.data[11]*m.data[12]) - (m.data[ 3]*m.data[ 8]*m.data[13])) * invdet,
        ((m.data[ 0]*m.data[ 5]*m.data[15]) + (m.data[ 1]*m.data[ 7]*m.data[12]) + (m.data[ 3]*m.data[ 4]*m.data[13]) - (m.data[ 0]*m.data[ 7]*m.data[13]) - (m.data[ 1]*m.data[ 4]*m.data[15]) - (m.data[ 3]*m.data[ 5]*m.data[12])) * invdet,
        ((m.data[ 0]*m.data[ 7]*m.data[ 9]) + (m.data[ 1]*m.data[ 4]*m.data[11]) + (m.data[ 3]*m.data[ 5]*m.data[ 8]) - (m.data[ 0]*m.data[ 5]*m.data[11]) - (m.data[ 1]*m.data[ 7]*m.data[ 8]) - (m.data[ 3]*m.data[ 4]*m.data[ 9])) * invdet,

        ((m.data[ 4]*m.data[10]*m.data[13]) + (m.data[ 5]*m.data[ 8]*m.data[14]) + (m.data[ 6]*m.data[ 9]*m.data[12]) - (m.data[ 4]*m.data[ 9]*m.data[14]) - (m.data[ 5]*m.data[10]*m.data[12]) - (m.data[ 6]*m.data[ 8]*m.data[13])) * invdet,
        ((m.data[ 0]*m.data[ 9]*m.data[14]) + (m.data[ 1]*m.data[10]*m.data[12]) + (m.data[ 2]*m.data[ 8]*m.data[13]) - (m.data[ 0]*m.data[10]*m.data[13]) - (m.data[ 1]*m.data[ 8]*m.data[14]) - (m.data[ 2]*m.data[ 9]*m.data[12])) * invdet,
        ((m.data[ 0]*m.data[ 6]*m.data[13]) + (m.data[ 1]*m.data[ 4]*m.data[14]) + (m.data[ 2]*m.data[ 5]*m.data[12]) - (m.data[ 0]*m.data[ 5]*m.data[14]) - (m.data[ 1]*m.data[ 6]*m.data[12]) - (m.data[ 2]*m.data[ 4]*m.data[13])) * invdet,
        ((m.data[ 0]*m.data[ 5]*m.data[10]) + (m.data[ 1]*m.data[ 6]*m.data[ 8]) + (m.data[ 2]*m.data[ 4]*m.data[ 9]) - (m.data[ 0]*m.data[ 6]*m.data[ 9]) - (m.data[ 1]*m.data[ 4]*m.data[10]) - (m.data[ 2]*m.data[ 5]*m.data[ 8])) * invdet
    }};
}

// Creates a new projection matrix using the provided settings
mat4 mat4_projection(projection_settings settings) {
    mat4 m = mat4_ident;

    if(settings.is_ortho)
    {
        m.data[0]  = -2.0f / (settings.dims.x);
        m.data[5]  = -2.0f / (-settings.dims.y);
        m.data[10] = -2.0f / (settings.dims.w - settings.dims.z);
        m.data[14] = (settings.dims.w + settings.dims.z) / (settings.dims.w - settings.dims.z) * -1;
        m.data[15] = 1;
    }
    else
    {
        float ratio = settings.dims.x / settings.dims.y;
        float tfov  = 1.0f / tan(degtorad(settings.fov * 0.5f));
        float near  = settings.dims.z;
        float far   = settings.dims.w;
        float zdiff = 1.0f/ (near - far);

        m.data[0]  = tfov / ratio;
        m.data[5]  = tfov;
        m.data[10] = (near + far) * zdiff;
        m.data[11] = -1;
        m.data[14] = 2.0f * far * near * zdiff;
        m.data[15] = 0;
    }

    return m;
}
