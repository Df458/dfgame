// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "window.h"

#include "camera.h"
#include "check.h"
#include "input.h"
#include "log/log.h"
#include "graphics_log.h"

#include <gtk/gtk.h>

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
