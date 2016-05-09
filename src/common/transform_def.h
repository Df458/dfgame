#ifndef DF_TRANSFORM_DEF
#define DF_TRANSFORM_DEF
#include "matrix.h"

struct transform2D
{
    float x;
    float y;

    float angle;

    float scale_x;
    float scale_y;

    mat4 matrix;
};

#endif
