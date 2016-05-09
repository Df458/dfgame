#ifndef DF_TRANSFORM_H
#define DF_TRANSFORM_H
#include "matrix.h"
#include <stdbool.h>

typedef struct transform2D transform2D;

transform2D* create_transform();
void destroy_transform(transform2D* trans);
void transform_set_position(transform2D* trans, float x, float y, bool relative);
void transform_set_angle(transform2D* trans, float angle, bool relative);
void transform_set_scale(transform2D* trans, float x, float y, bool relative);
void transform_get_position(transform2D* trans, float* x, float* y);
void transform_get_angle(transform2D* trans, float* angle);
void transform_get_scale(transform2D* trans, float* x, float* y);
mat4 transform_get_matrix(transform2D* trans);
mat4 transform_get_view_matrix(transform2D* trans);

#endif
