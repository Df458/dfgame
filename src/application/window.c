// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "window.h"

#include "check.h"
#include "input.h"
#include "log/log.h"
#include "graphics_log.h"

static bool glfw_init_called = false;

// Creates a new GLFWwindow with an OpenGL context and input callbacks.
// This also calls glfwInit and glewInit the first time that it's called.
GLFWwindow* window_new_default(uint16 width, uint16 height, const char* title) {
    check_kill(glfw_init_called || glfwInit(), "Failed to intialize GLFW.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSwapInterval(0);
    
    GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwSetKeyCallback(win, input_key_callback);
    glfwSetMouseButtonCallback(win, input_mouse_button_callback);
    glfwSetScrollCallback(win, input_mouse_scroll_callback);
    glfwSetCursorPosCallback(win, input_mouse_position_callback);

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

    glfw_init_called = true;

    return win;
}

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(GLFWwindow* win) {
    glfwDestroyWindow(win);
}
void _window_free_final(GLFWwindow* win) {
    window_free(win);
    glfwTerminate();
    glfw_init_called = false;
}
