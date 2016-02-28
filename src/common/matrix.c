#include "matrix.h"

#include <math.h>
#include <string.h>

mat4 ident_data= ident;

mat4 ortho(float left, float right, float bottom, float top, float near, float far)
{
    mat4 mat = ident;
    mat.data[0]  = 2.0f / (right - left);
    mat.data[12]  = (right + left) / (right - left) * -1;
    mat.data[5]  = 2.0f / (top - bottom);
    mat.data[13]  = (top + bottom) / (top - bottom) * -1;
    mat.data[10] = 2.0f / (far - near);
    mat.data[14] = (far + near) / (far - near) * -1;
    mat.data[15] = 1;

    return mat;
}

mat4 mul(mat4 lv, mat4 rv)
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

void translate(mat4* mat, float position_x, float position_y, uint8_t relative)
{
    if(relative) {
        mat->data[12] += position_x;
        mat->data[13] += position_y;
    } else {
        mat->data[12] = position_x;
        mat->data[13] = position_y;
    }
}

void rotate(mat4* mat, float angle, uint8_t relative)
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

void scale(mat4* mat, float scale_x, float scale_y, uint8_t relative)
{
    // TODO: Make relative vs. absolute
    mat->data[0] *= scale_x;
    mat->data[5] *= scale_y;
}

void mat_reset(mat4* mat)
{
    memcpy(mat->data, ident_data.data, 16 * sizeof(float));
}
