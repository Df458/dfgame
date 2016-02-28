#ifndef INPUT_SOURCE_H
#define INPUT_SOURCE_H
#include <stdbool.h>
#include <inttypes.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool init_input(GLFWwindow* win);

bool map_key_to_action(int key, int mods, uint8_t action);
bool map_key_to_axis(int key, int mods, uint8_t axis, bool isx, bool positive, bool analog);
bool unmap_key(int key, int mods);

// TODO: Set the correct args
bool map_joy_button_to_action();
bool map_joy_button_to_axis();
bool unmap_joy_button();

// TODO: Set the correct args
bool map_joy_axis_to_action();
bool map_joy_axis_to_axis();
bool unmap_joy_axis();

bool map_mouse_button_to_action(int button, int mods, uint8_t action);
bool map_mouse_button_to_axis(int button, int mods, uint8_t action, bool analog);
bool unmap_mouse_button(int button, int mods);

// TODO: Set the correct args
bool map_mouse_motion_to_action();
bool map_mouse_motion_to_axis();
bool unmap_mouse_motion();

#endif
