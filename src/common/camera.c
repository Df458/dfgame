#include "camera.h"
#include "util.h"

camera* create_camera_2D()
{
    camera* cam = salloc(sizeof(camera));
    cam->projection = create_mat4();
    cam->transform = create_transform();

    return cam;
}

void destroy_camera_full(camera* cam)
{
    destroy_transform(cam->transform);
    sfree(cam);
}

// TODO: Only recalculate this matrix if things have changed
mat4 camera_get_matrix(camera* cam)
{
    return mat4_mul(cam->projection, transform_get_view_matrix(cam->transform));
}

mat4 camera_get_view(camera* cam)
{
    return transform_get_view_matrix(cam->transform);
}

mat4 camera_get_projection(camera* cam)
{
    return cam->projection;
}
