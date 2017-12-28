// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "window.h"

#include "camera.h"
#include "check.h"
#include "input.h"
#include "log/log.h"
#include "graphics_log.h"

#include <gtk/gtk.h>

extern double __application_editor_mouse_x, __application_editor_mouse_y;

camera window_create_2d_camera(void* win) {
    int w = gtk_widget_get_allocated_width((GtkWidget*)win);
    int h = gtk_widget_get_allocated_height((GtkWidget*)win);

    projection_settings settings = (projection_settings) {
        .dims = {
            .x = w,
            .y = h,
            .z = -1,
            .w = 100
        },
        .is_ortho = true
    };

    return camera_new(settings);
}

vec2 get_mouse_position_raw(void* win) {
    return (vec2){ .x=__application_editor_mouse_x, .y=__application_editor_mouse_y };
}
vec2 get_mouse_position(void* win) {
    vec2 v = get_mouse_position_raw(win);
    vec2 v2 = get_window_dims(win);
    return (vec2){ .x = v.x / v2.x * 2 - 1, .y = v.y / v2.y * -2 + 1 };
}
vec2 get_window_dims(void* win) {
    int x = gtk_widget_get_allocated_width((GtkWidget*)win);
    int y = gtk_widget_get_allocated_height((GtkWidget*)win);
    return (vec2){ .x=x, .y=y };
}
