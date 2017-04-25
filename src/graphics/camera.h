#ifndef DF_GRAPHICS_CAMERA
#define DF_GRAPHICS_CAMERA
#include "camera.hd"

#include "matrix.h"
#include "transform.hd"
#include "vector.h"

// Creates a new camera
camera camera_new(projection_settings settings);

// Gets the transform attached to c
transform camera_get_transform(camera c);

// Returns the projection matrix associated with c
mat4 camera_get_projection(camera c);

// Sets the camera's projection from the provided settings
void camera_set_projection(camera c, projection_settings settings);

// Calculates and returns the view matrix of c
mat4 camera_get_view(camera c);

// Calculates and returns the multiplied view/projection matrices of c
mat4 camera_get_vp(camera c);

// Frees the camera
#define camera_free(c) { _camera_free(c); c = NULL; }
void _camera_free(camera c);

#endif
