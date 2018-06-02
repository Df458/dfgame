#ifndef DF_APPLICATION_WINDOW
#define DF_APPLICATION_WINDOW

#include "core/types.h"
#include "math/vector.h"
#include "graphics/camera.hd"

#include "application/input_id.h"
#include "application/window_common.h"

// Creates a new window with an OpenGL context.
// This also calls glewInit for the new context.
window window_new(uint16 width, uint16 height, bool resizable, const char* title);

// Returns whether or not the window has received a close request.
bool window_get_should_close(window win);

// Gets/sets the cursor mode, indicating how the mouse is tracked and displayed
cursor_mode window_get_cursor_mode(window win);
void window_set_cursor_mode(window win, cursor_mode mode);

// Frees the window.
// window_free_final also calls glfwTerminate.
// NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _window_free(window win);
#define window_free(win) { _window_free(win); win = NULL; }
void _window_free_final(window win);
#define window_free_final(win) { _window_free_final(win); win = NULL; }

#endif
