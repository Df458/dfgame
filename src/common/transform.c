#include "transform_def.h"
#include "transform.h"

#include <stdlib.h>

// Private functions
static void recalculate_matrix(transform2D* trans)
{
    mat_reset(&trans->matrix);
    translate(&trans->matrix, trans->x, trans->y, 0);
    rotate(&trans->matrix, trans->angle, 0);
    scale(&trans->matrix, trans->scale_x, trans->scale_y, 0);
}

// Public functions
transform2D* create_transform()
{
    transform2D* trans = malloc(sizeof(transform2D));
    trans->x = 0;
    trans->y = 0;
    trans->angle = 0;
    trans->scale_x = 1;
    trans->scale_y = 1;

    return trans;
}

void destroy_transform(transform2D* trans)
{
    free(trans);
}

void transform_set_position(transform2D* trans, float x, float y, bool relative)
{
    if(relative) {
        trans->x += x;
        trans->y += y;
    } else {
        trans->x = x;
        trans->y = y;
    }

    recalculate_matrix(trans);
}

void transform_set_angle(transform2D* trans, float angle, bool relative)
{
    if(relative)
        trans->angle += angle;
    else
        trans->angle = angle;

    recalculate_matrix(trans);
}

void transform_set_scale(transform2D* trans, float x, float y, bool relative)
{
    if(relative) {
        trans->scale_x += x;
        trans->scale_y += y;
    } else {
        trans->scale_x = x;
        trans->scale_y = y;
    }

    recalculate_matrix(trans);
}

mat4 transform_get_matrix(transform2D* trans)
{
    return trans->matrix;
}
