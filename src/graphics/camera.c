// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "matrix.h"
#include "memory/alloc.h"
#include "transform.h"

#include "camera.h"

// Represents a camera with a transform and projection matrix
typedef struct camera {
    mat4 projection;
    transform view;
}* camera;

// Creates a new camera
camera camera_new(projection_settings settings) {
    camera c = salloc(sizeof(struct camera));

    c->view = transform_new();
    camera_set_projection(c, settings);

    return c;
}

// Gets the transform attached to c
transform camera_get_transform(camera c) {
    return c->view;
}

// Returns the projection matrix associated with c
mat4 camera_get_projection(camera c) {
    return c->projection;
}

// Sets the camera's projection from the provided settings
void camera_set_projection(camera c, projection_settings settings) {
    c->projection = mat4_projection(settings);
}

// Calculates and returns the view matrix of c
mat4 camera_get_view(camera c) {
    return mat4_invert(transform_get_matrix(c->view));
}

// Calculates and returns the multiplied view/projection matrices of c
mat4 camera_get_vp(camera c) {
    return mat4_mul(c->projection, camera_get_view(c));
}

// Frees the camera
void _camera_free(camera c) {
    if(c != NULL)
        transform_free(c->view);
    sfree(c);
}
