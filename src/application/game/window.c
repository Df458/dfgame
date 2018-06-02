// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "application/game/window.h"

#include "application/game/glfw_input.h"
#include "core/check.h"
#include "core/log/log.h"
#include "graphics/camera.h"
#include "graphics/graphics_log.h"

static bool glfw_init_called = false;

void window_framebuffer_size_callback(GLFWwindow* win, int w, int h) {
    window wrapper = glfwGetWindowUserPointer(win);

    // Forward the event to user code
    call_event(wrapper->resize_event, w, h);
}

// Creates a new window with an OpenGL context.
// This also calls glewInit for the new context.
window window_new(uint16 width, uint16 height, bool resizable, const char* title) {
    check_kill(glfw_init_called || glfwInit(), "Failed to intialize GLFW.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!win) {
        glfwTerminate();
        fatal("Failed to create window");
    }

    window w = msalloc(struct window);
    w->platform_data = win;
    w->resize_event = NULL;

    glfwSetKeyCallback(win, input_key_callback);
    glfwSetMouseButtonCallback(win, input_mouse_button_callback);
    glfwSetScrollCallback(win, input_mouse_scroll_callback);
    glfwSetCursorPosCallback(win, input_mouse_position_callback);
    glfwSetFramebufferSizeCallback(win, window_framebuffer_size_callback);

    // This allows us to access window events from glfw events
    glfwSetWindowUserPointer(win, w);

    if(!resizable)
        glfwSetWindowSizeLimits(win, width, height, width, height);

    glfwMakeContextCurrent(win);

    check_kill(glewInit() == GLEW_OK, "Failed to initialize GLEW");
    if(!check_error(glfwExtensionSupported("GL_ARB_debug_output"), "Graphics debug logging is not available for your hardware. Graphical errors will not be reported")) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(graphics_log, NULL);
    }

    // Setup default VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up some simple defaults
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glfw_init_called = true;

    return w;
}

// Tells the context to draw, usually by swapping buffers
void window_redraw(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return;
    }

    glfwSwapBuffers((GLFWwindow*)win->platform_data);
}

// Returns whether or not the window has received a close request.
bool window_get_should_close(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return false;
    }

    return glfwWindowShouldClose((GLFWwindow*)win->platform_data);
}

// Gets/sets the cursor mode, indicating how the mouse is tracked and displayed
cursor_mode window_get_cursor_mode(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return CM_ENABLED;
    }

    int mode = glfwGetInputMode(win->platform_data, GLFW_CURSOR);
    switch(mode) {
        case GLFW_CURSOR_NORMAL:
            return CM_ENABLED;
            break;
        case GLFW_CURSOR_HIDDEN:
            return CM_HIDDEN;
            break;
        case GLFW_CURSOR_DISABLED:
            return CM_DISABLED;
            break;
    }

    warn("GLFW cursor mode 0x%x is unknown", mode);

    return CM_ENABLED;
}
void window_set_cursor_mode(window win, cursor_mode mode) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return;
    }

    switch(mode) {
        case CM_ENABLED:
            glfwSetInputMode(win->platform_data, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CM_HIDDEN:
            glfwSetInputMode(win->platform_data, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case CM_DISABLED:
            glfwSetInputMode(win->platform_data, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
    }
}

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return;
    }

    glfwDestroyWindow((GLFWwindow*)win->platform_data);

    if(win->resize_event)
        sfree(win->resize_event);

    sfree(win);
}
void _window_free_final(window win) {
    _window_free(win);

    glfwTerminate();
    glfw_init_called = false;
}

// Creates a new camera based on the window's dimensions.
camera window_create_2d_camera(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return NULL;
    }

    int w, h;
    glfwGetWindowSize((GLFWwindow*)win->platform_data, &w, &h);

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

// Returns the mouse position in pixels.
vec2 window_get_mouse_position_raw(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return (vec2){0};
    }

    double x, y;
    glfwGetCursorPos(win->platform_data, &x, &y);
    return (vec2){ .x = x, .y = y };
}

// Returns the mouse position in screen coordinates.
vec2 window_get_mouse_position(window win) {
    vec2 v = window_get_mouse_position_raw(win);
    vec2 v2 = window_get_dims(win);

    if(check_warn(eq0(v2.x) || eq0(v2.y), "Failed to calculate mouse position")) {
        return (vec2){0};
    }

    return (vec2){ .x = v.x / v2.x * 2 - 1, .y = v.y / v2.y * -2 + 1 };
}

// Returns the dimensions of the window.
vec2 window_get_dims(window win) {
    if(check_warn(win, "Window is NULL") || check_warn(win->platform_data, "Window is invalid")) {
        return (vec2){0};
    }

    int x, y;
    glfwGetWindowSize(win->platform_data, &x, &y);
    return (vec2){ .x = x, .y = y };
}
