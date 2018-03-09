// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "window.h"

#include "camera.h"
#include "check.h"
#include "glfw_input.h"
#include "log/log.h"
#include "graphics_log.h"

static bool glfw_init_called = false;

// Creates a new GLFWwindow with an OpenGL context and input callbacks.
// This also calls glfwInit and glewInit the first time that it's called.
void* window_new_default(uint16 width, uint16 height, const char* title) {
    check_kill(glfw_init_called || glfwInit(), "Failed to intialize GLFW.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSwapInterval(0);
    
    GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!win) {
        glfwTerminate();
        fatal("Failed to create window");
    }
    glfwSetKeyCallback(win, input_key_callback);
    glfwSetMouseButtonCallback(win, input_mouse_button_callback);
    glfwSetScrollCallback(win, input_mouse_scroll_callback);
    glfwSetCursorPosCallback(win, input_mouse_position_callback);

    glfwSetWindowSizeLimits(win, width, height, width, height);

    check_kill(win, "Failed to create a new window");
    glfwMakeContextCurrent(win);

    check_kill(glfw_init_called || glewInit() == GLEW_OK, "Failed to initialize GLEW");
    if(!glfw_init_called) {
        if(!check_error(glfwExtensionSupported("GL_ARB_debug_output"), "Graphics debug logging is not available for your hardware. Graphical errors will not be reported")) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(graphics_log, NULL);
        }

        GLuint VAO; // Setup default VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glfw_init_called = true;

    return win;
}

void window_redraw(void* win) {
    glfwSwapBuffers((GLFWwindow*)win);
}

bool window_should_close(void* win) {
    return glfwWindowShouldClose((GLFWwindow*)win);
}

void window_cursor_mode(void* win, cursor_mode mode) {
    switch(mode) {
        case CM_ENABLED:
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CM_HIDDEN:
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case CM_DISABLED:
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
    }
}

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(void* win) {
    glfwDestroyWindow((GLFWwindow*)win);
}
void _window_free_final(void* win) {
    window_free(win);
    glfwTerminate();
    glfw_init_called = false;
}

camera window_create_2d_camera(void* win) {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)win, &w, &h);

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
    double x, y;
    glfwGetCursorPos(win, &x, &y);
    return (vec2){ .x=x, .y=y };
}
vec2 get_mouse_position(void* win) {
    vec2 v = get_mouse_position_raw(win);
    vec2 v2 = get_window_dims(win);
    return (vec2){ .x = v.x / v2.x * 2 - 1, .y = v.y / v2.y * -2 + 1 };
}
vec2 get_window_dims(void* win) {
    int x, y;
    glfwGetWindowSize(win, &x, &y);
    return (vec2){ .x=x, .y=y };
}
