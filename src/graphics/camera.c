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
camera camera_new() {
    camera c = salloc(sizeof(struct camera));

    float left = -2.0f;
    float right = 2.0f;
    float top = -2.0f;
    float bottom = 2.0f;
    float near = -2.0f;
    float far = 2.0f;
    c->view = transform_new();
    c->projection = mat4_ident;
    c->projection.data[0]  = 2.0f / (right - left);
    c->projection.data[12]  = (right + left) / (right - left) * -1;
    c->projection.data[5]  = 2.0f / (top - bottom);
    c->projection.data[13]  = (top + bottom) / (top - bottom) * -1;
    c->projection.data[10] = 2.0f / (far - near);
    c->projection.data[14] = (far + near) / (far - near) * -1;
    c->projection.data[15] = 1;

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

// Calculates and returns the view matrix of c
mat4 camera_get_view(camera c) {
    return mat4_invert(transform_get_matrix(c->view));
}

// Calculates and returns the multiplied view/projection matrices of c
mat4 camera_get_vp(camera c) {
    return mat4_mul(mat4_invert(transform_get_matrix(c->view)), c->projection);
}

// Frees the camera
void _camera_free(camera c) {
    if(c != NULL)
        transform_free(c->view);
    sfree(c);
}
