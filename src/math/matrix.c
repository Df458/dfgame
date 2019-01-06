// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "matrix.h"

#include "check.h"
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
    m2.data[15] = v.w;

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
    my.data[8] = -sy;
    my.data[10] = cy;

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
mat4 mat4_scale_f(mat4 m, float v) {
    mat4 m2 = m;
    m2.data[0]  *= v;
    m2.data[5]  *= v;
    m2.data[10] *= v;

    return m2;
}
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
mat4 mat4_mul_mat4(mat4 m1, mat4 m2) {
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
vec4 mat4_mul_vec2(mat4 m1, vec2 v) {
    return mat4_mul_vec4(m1, (vec4){ .x=v.x, .y=v.y, .z=0, .w=1 });
}
vec4 mat4_mul_vec3(mat4 m1, vec3 v) {
    return mat4_mul_vec4(m1, (vec4){ .x=v.x, .y=v.y, .z=v.z, .w=1 });
}
vec4 mat4_mul_vec4(mat4 m1, vec4 v) {
    vec4 res;
    res.x = m1.data[0] * v.x + m1.data[4] * v.y + m1.data[8] * v.z + m1.data[12] * v.w;
    res.y = m1.data[1] * v.x + m1.data[5] * v.y + m1.data[9] * v.z + m1.data[13] * v.w;
    res.z = m1.data[2] * v.x + m1.data[6] * v.y + m1.data[10] * v.z + m1.data[14] * v.w;
    res.w = m1.data[3] * v.x + m1.data[7] * v.y + m1.data[11] * v.z + m1.data[15] * v.w;
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
        m.data[0]  = 2.0f / (settings.dims.x);
        m.data[5]  = 2.0f / (-settings.dims.y);
        m.data[10] = 2.0f / (settings.dims.w - settings.dims.z);
        m.data[14] = (settings.dims.w + settings.dims.z) / (settings.dims.w - settings.dims.z) * -1;
        m.data[15] = 1;
    }
    else
    {
        float ratio = settings.dims.x / settings.dims.y;
        float tfov  = 1.0f / tan(degtorad(settings.fov * 0.5f));
        float zdiff = 1.0f/ (settings.dims.z - settings.dims.w);

        m.data[0]  = tfov / ratio;
        m.data[5]  = tfov;
        m.data[10] = (settings.dims.z + settings.dims.w) * zdiff;
        m.data[11] = -1;
        m.data[14] = 2.0f * settings.dims.w * settings.dims.z * zdiff;
        m.data[15] = 0;
    }

    return m;
}

// Creates a rotation matrix that faces from eye to target
mat4 mat4_lookat(vec3 eye, vec3 target, vec3 up) {
    vec3 f = vec3_sub(eye, target);
    if(eq0(vec3_len_squared(f)))
        return mat4_ident;

    f = vec3_normalize(f);

    vec3 s = vec3_cross(f, vec3_normalize(up));
    vec3 u = vec3_cross(vec3_normalize(s), f);

    return (mat4) { .data = {
         s.x,  s.y,  s.z, 0,
         u.x,  u.y,  u.z, 0,
        -f.x, -f.y, -f.z, 0,
         0,    0,    0,   1
    }};
}

// Converts a rotation matrix into a quaternion
quat mat4_to_quat(mat4 m) {
    float t = 1;
    quat q;
    if(m.data[10] < 0) {
        if(m.data[0] > m.data[5]) {
            t = 1 + m.data[0] - m.data[5] - m.data[10];
            q = (quat){ .x=t, .y=m.data[4]+m.data[1], .z=m.data[2]+m.data[8], .w=m.data[9]-m.data[6]};
        } else {
            t = 1 - m.data[0] + m.data[5] - m.data[10];
            q = (quat){ .x=m.data[4]+m.data[1], .y=t, .z=m.data[9]+m.data[6], .w=m.data[2]-m.data[8]};
        }
    } else {
        if(m.data[0] < -m.data[5]) {
            t = 1 - m.data[0] - m.data[5] + m.data[10];
            q = (quat){ .x=m.data[2]+m.data[8], .y=m.data[9]+m.data[6], .z=t, .w=m.data[4]-m.data[1]};
        } else {
            t = 1 + m.data[0] + m.data[5] + m.data[10];
            q = (quat){
                .x=m.data[9]-m.data[6],
                .y=m.data[2]-m.data[8],
                .z=m.data[4]-m.data[1],
                .w=t};
        }
    }
    return quat_mul(q, 0.5f / sqrt(t));
}

vec3 mat4_to_euler(mat4 m) {
    float sy = sqrt(square(m.data[0]) + square(m.data[1]));

    bool singular = sy < 1e-6;

    float x, y, z;
    if (!singular)
    {
        x = atan2(m.data[6], m.data[7]);
        y = atan2(-m.data[8], sy);
        z = atan2(m.data[1], m.data[0]);
    }
    else
    {
        x = atan2(-m.data[6], m.data[5]);
        y = atan2(-m.data[8], sy);
        z = 0;
    }
    return (vec3){.data={x,y,z}};
}

/* bool mat4_rotation_valid(mat4 m) { */
/*     mat4 mt = mat4_transpose(m); */
/*     mat4 ident = mat4_mul(mt, m); */
/*     #<{(| float val = mat4_norm(ident, mat4_ident); |)}># */
/*     float val = fabs((1-ident.data[0]) + ident.data[1] + ident.data[2] + ident.data[4] + (1-ident.data[5]) + ident.data[6] + ident.data[8] + ident.data[9] + (1-ident.data[10])); */
/*     info("[[]]%f\n" mat4_printstr, val, mat4_printargs(ident)); */
/*     return val < 0.000001; */
/* } */
