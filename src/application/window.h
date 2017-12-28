#ifndef DF_APPLICATION_WINDOW
#define DF_APPLICATION_WINDOW

#include "camera.hd"
#include "types.h"
#include "vector.h"

#include "input_id.h"

// Creates a new GLFWwindow with an OpenGL context.
// This also calls glfwInit and glewInit the first time that it's called.
void* window_new_default(uint16 width, uint16 height, const char* title);

void window_redraw(void* win);

bool window_should_close(void* win);

void window_cursor_mode(void* win, cursor_mode mode);

// Frees the window.
// window_free_final also calls glfwTerminate.
#define window_free(win) { _window_free(win); win = NULL; }
#define window_free_final(win) { _window_free_final(win); win = NULL; }

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(void* win);
void _window_free_final(void* win);

camera window_create_2d_camera(void* win);

vec2 get_mouse_position_raw(void* win);
vec2 get_mouse_position(void* win);
vec2 get_window_dims(void* win);

#endif
