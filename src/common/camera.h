#ifndef DF_CAMERA_H
#define DF_CAMERA_H

#include "transform.h"
#include "matrix.h"

typedef struct camera2D
{
    transform2D* transform;
    mat4         projection;
}
camera;

camera* create_camera_2D();
void destroy_camera_full(camera* cam);
#define destroy_camera(camera) { destroy_camera_full(camera); camera = 0; }
#define camera_ortho(cam, left, right, bottom, top, near, far) mat4_ortho(&cam->projection, left, right, bottom, top, near, far)
#define camera_set_position(cam, x, y, rel) transform_set_position(cam->transform, x, y, rel)
#define camera_set_angle(cam, angle, rel) transform_set_angle(cam->transform, angle, rel)
#define camera_set_scale(cam, x, y, rel) transform_set_scale(cam->transform, x, y, rel)
#define camera_get_position(cam, x, y) transform_get_position(cam->transform, x, y);
#define camera_get_angle(cam, angle) transform_get_angle(cam->transform, angle);
#define camera_get_scale(cam, x, y) transform_get_scale(cam->transform, x, y);
mat4 camera_get_matrix(camera* cam);

#endif
