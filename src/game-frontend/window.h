#ifndef DF_WINDOW_H
#define DF_WINDOW_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <inttypes.h>

/*!
 * Creates a GLFW window. Connects some callbacks, and inits GLFW/GLEW.
 */
GLFWwindow* create_main_window(uint16_t width, uint16_t height, const char* title);

/*!
 * Gets the main window, if it exists.
 */
GLFWwindow* get_main_window();

/*!
 * Sets an additional callback to run when the main window is resized.
 */
void set_window_size_func(GLFWframebuffersizefun func);

/*!
 * Destroys the main window, and ends GLFW
 */
void cleanup_main_window();

#endif
