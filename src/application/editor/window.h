#ifndef DF_APPLICATION_WINDOW
#define DF_APPLICATION_WINDOW

#include "core/types.h"
#include "math/vector.h"
#include "graphics/camera.hd"

#include "application/input_id.h"
#include "application/window_common.h"

typedef struct window_proxy {
    vec2 mouse;
    vec2 dims;

    void* widget;
}* window_proxy;

// Creates a new window struct from a DFGameViewport.
// This also calls glewInit for the viewport's context.
window window_new(void* widget);

void window_free(window win);

#endif
