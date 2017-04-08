#ifndef DF_GRAPHICS_CAMERA
#define DF_GRAPHICS_CAMERA
#include "camera.hd"

#include "matrix.hd"
#include "transform.hd"

// Creates a new camera
camera camera_new();

// Gets the transform attached to c
transform camera_get_transform(camera c);

// Returns the projection matrix associated with c
mat4 camera_get_projection(camera c);

// Calculates and returns the view matrix of c
mat4 camera_get_view(camera c);

// Calculates and returns the multiplied view/projection matrices of c
mat4 camera_get_vp(camera c);

// Frees the camera
#define camera_free(c) { _camera_free(c); c = NULL; }
void _camera_free(camera c);

#endif
