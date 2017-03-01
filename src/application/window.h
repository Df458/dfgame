#ifndef DF_APPLICATION_WINDOW
#define DF_APPLICATION_WINDOW

#include "types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Creates a new GLFWwindow with an OpenGL context.
// This also calls glfwInit and glewInit the first time that it's called.
GLFWwindow* window_new_default(uint16 width, uint16 height, const char* title);

// Frees the window.
// window_free_final also calls glfwTerminate.
#define window_free(win) { _window_free(win); win = NULL; }
#define window_free_final(win) { _window_free_final(win); win = NULL; }

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(GLFWwindow* win);
void _window_free_final(GLFWwindow* win);

#endif
