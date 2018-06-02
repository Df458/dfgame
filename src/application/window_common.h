#ifndef DF_APPLICATION_WINDOW_COMON
#define DF_APPLICATION_WINDOW_COMON

#include "core/types.h"
#include "math/vector.h"
#include "graphics/camera.hd"

#include "application/input_id.h"

event(window_resize, uint16 width, uint16 height);

typedef struct window {
    void* platform_data;

    window_resize* resize_event;
} *window;

// Tells the context to draw, usually by swapping buffers
void window_redraw(window win);

// Creates a new camera based on the window's dimensions.
camera window_create_2d_camera(window win);

// Returns the mouse position in pixels.
vec2 window_get_mouse_position_raw(window win);

// Returns the mouse position in screen coordinates.
vec2 window_get_mouse_position(window win);

// Returns the dimensions of the window.
vec2 window_get_dims(window win);

#endif
