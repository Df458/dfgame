// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "application/editor/window.h"

#include "application/input.h"
#include "core/check.h"
#include "core/log/log.h"
#include "graphics/camera.h"
#include "graphics/graphics_log.h"

#include <gtk/gtk.h>

window window_new(void* widget) {
    check_return(widget, "Trying to create a window from a NULL viewport", NULL);

    window w = msalloc(struct window);
    window_proxy edw = msalloc(struct window_proxy);
    edw->mouse.x = -1;
    edw->mouse.y = -1;
    edw->dims = (vec2){ 0 };
    edw->widget = widget;
    w->platform_data = edw;
    w->resize_event = NULL;

    return w;
}

// Tells the context to draw, usually by swapping buffers
void window_redraw(window win) {
    // This function is a no-op, as GTK handles rendering.
    // We still want the function recognized by the API, however.
}

// Creates a new camera based on the window's dimensions.
camera window_create_2d_camera(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return NULL;
    }

    vec2 dims = window_get_dims(win);

    projection_settings settings = (projection_settings) {
        .dims = {
            .x = dims.x,
            .y = dims.y,
            .z = -1,
            .w = 100
        },
        .is_ortho = true
    };

    return camera_new(settings);
}

// Returns the mouse position in pixels.
vec2 window_get_mouse_position_raw(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return (vec2){0};
    }

    return ((window_proxy)win->platform_data)->mouse;
}

// Returns the mouse position in screen coordinates.
vec2 window_get_mouse_position(window win) {
    vec2 v = window_get_mouse_position_raw(win);
    vec2 v2 = window_get_dims(win);

    if(check_warn(!eq0(v2.x) && !eq0(v2.y), "Failed to calculate mouse position")) {
        return (vec2){0};
    }

    return (vec2){ .x = v.x / v2.x * 2 - 1, .y = v.y / v2.y * -2 + 1 };
}

// Returns the dimensions of the window.
vec2 window_get_dims(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return (vec2){0};
    }

    return ((window_proxy)win->platform_data)->dims;
}

void window_free(window win) {
    // TODO
}
