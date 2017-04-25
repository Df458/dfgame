#ifndef DF_APPLICATION_INPUT
#define DF_APPLICATION_INPUT

#include "control.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void input_bind_key_action(int key, action_id id);
void input_bind_mouse_button_action(int button, action_id id);
action_id input_add_key_action(int key, action_event* event);
action_id input_add_mouse_button_action(int key, action_event* event);

void input_bind_key_axis(int key, axis_id id, float mod);
void input_bind_mouse_button_axis(int button, axis_id id, float mod);
void input_bind_mouse_scroll_axis(bool vertical, axis_id id, float mod);
void input_bind_mouse_position_axis(bool vertical, axis_id id, float mod);
axis_id input_add_key_axis(int key, float limit, float mod);
axis_id input_add_mouse_button_axis(int button, float limit, float mod);
axis_id input_add_mouse_scroll_axis(bool vertical, float limit, float mod);
axis_id input_add_mouse_position_axis(bool vertical, float limit, float mod);

void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void input_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void input_mouse_position_callback(GLFWwindow* window, double xoffset, double yoffset);

void update_input();

#endif
