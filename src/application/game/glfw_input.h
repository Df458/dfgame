#ifndef DF_GLFW_INPUT_H
#define DF_GLFW_INPUT_H

#include "application/input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void input_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void input_mouse_position_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif
