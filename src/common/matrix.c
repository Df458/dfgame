#include "matrix.h"
#include "util.h"

#include <math.h>
#include <string.h>
#include <inttypes.h>

mat4 ident_data = ident;

mat4 create_mat4()
{
    mat4 mat;
    mat4_reset(&mat);
    return mat;
}

void mat4_ortho(mat4* mat, float left, float right, float bottom, float top, float near, float far)
{
    mat4_reset(mat);
    mat->data[0]  = 2.0f / (right - left);
    mat->data[12]  = (right + left) / (right - left) * -1;
    mat->data[5]  = 2.0f / (top - bottom);
    mat->data[13]  = (top + bottom) / (top - bottom) * -1;
    mat->data[10] = 2.0f / (far - near);
    mat->data[14] = (far + near) / (far - near) * -1;
    mat->data[15] = 1;
}

mat4 mat4_mul(mat4 lv, mat4 rv)
{
    mat4 res;
    for(uint8_t i = 0; i < 4; ++i) {
        uint8_t j = i * 4;
        res.data[j    ] = lv.data[0]*rv.data[j] + lv.data[4]*rv.data[j+1] + lv.data[8]*rv.data[j+2] + lv.data[12]*rv.data[j+3];
        res.data[j + 1] = lv.data[1]*rv.data[j] + lv.data[5]*rv.data[j+1] + lv.data[9]*rv.data[j+2] + lv.data[13]*rv.data[j+3];
        res.data[j + 2] = lv.data[2]*rv.data[j] + lv.data[6]*rv.data[j+1] + lv.data[10]*rv.data[j+2] + lv.data[14]*rv.data[j+3];
        res.data[j + 3] = lv.data[3]*rv.data[j] + lv.data[7]*rv.data[j+1] + lv.data[11]*rv.data[j+2] + lv.data[15]*rv.data[j+3];
    }
    return res;
}

void mat4_translate(mat4* mat, float position_x, float position_y, bool relative)
{
    if(relative) {
        mat->data[12] += position_x;
        mat->data[13] += position_y;
    } else {
        mat->data[12] = position_x;
        mat->data[13] = position_y;
    }
}

// TODO: Relative support
void mat4_rotate(mat4* mat, float angle, bool relative)
{
    //  cos sin
    // -sin cos
    // ^-Correct matrix layout(upper-left corner)
    float s = sin(angle);
    float c = cos(angle);

    mat->data[0] =  c;
    mat->data[1] =  s;
    mat->data[4] = -s;
    mat->data[5] =  c;
}

// TODO: Make relative vs. absolute
void mat4_scale(mat4* mat, float scale_x, float scale_y, bool relative)
{
    mat->data[0] *= scale_x;
    mat->data[5] *= scale_y;
}

void mat4_reset(mat4* mat)
{
    memcpy(mat->data, ident_data.data, 16 * sizeof(float));
}

// TODO: Finish implementing this
void mat4_invert(mat4* mat)
{
    float det = mat4_get_determinant(*mat);
    if(det == 0)
        return;
    stub();
}

float mat4_get_determinant(mat4 mat)
{
    return mat.data[12] * mat.data[9] * mat.data[6]  * mat.data[3]  - mat.data[8] * mat.data[13] * mat.data[6]  * mat.data[3]  -
           mat.data[12] * mat.data[5] * mat.data[10] * mat.data[3]  + mat.data[4] * mat.data[13] * mat.data[10] * mat.data[3]  +
           mat.data[8]  * mat.data[5] * mat.data[14] * mat.data[3]  - mat.data[4] * mat.data[9]  * mat.data[14] * mat.data[3]  -
           mat.data[12] * mat.data[9] * mat.data[2]  * mat.data[7]  + mat.data[8] * mat.data[13] * mat.data[2]  * mat.data[7]  +
           mat.data[12] * mat.data[1] * mat.data[10] * mat.data[7]  - mat.data[0] * mat.data[13] * mat.data[10] * mat.data[7]  -
           mat.data[8]  * mat.data[1] * mat.data[14] * mat.data[7]  + mat.data[0] * mat.data[9]  * mat.data[14] * mat.data[7]  +
           mat.data[12] * mat.data[5] * mat.data[2]  * mat.data[11] - mat.data[4] * mat.data[13] * mat.data[2]  * mat.data[11] -
           mat.data[12] * mat.data[1] * mat.data[6]  * mat.data[11] + mat.data[0] * mat.data[13] * mat.data[6]  * mat.data[11] +
           mat.data[4]  * mat.data[1] * mat.data[14] * mat.data[11] - mat.data[0] * mat.data[5]  * mat.data[14] * mat.data[11] -
           mat.data[8]  * mat.data[5] * mat.data[2]  * mat.data[15] + mat.data[4] * mat.data[9]  * mat.data[2]  * mat.data[15] +
           mat.data[8]  * mat.data[1] * mat.data[6]  * mat.data[15] - mat.data[0] * mat.data[9]  * mat.data[6]  * mat.data[15] -
           mat.data[4]  * mat.data[1] * mat.data[10] * mat.data[15] + mat.data[0] * mat.data[5]  * mat.data[10] * mat.data[15];
}

void mat4_transpose(mat4* mat)
{
    for(int i = 0; i < 3; ++i) {
        for(int j = i + 1; j < 4; ++j) {
            float temp = mat->data[i * 4 + j];
            mat->data[i * 4 + j] = mat->data[j * 4 + i];
            mat->data[j * 4 + i] = temp;
        }
    }
}
