// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "glfw_input.h"

#include "glfw_keymap.h"

#include "application/input.priv.h"
#include "core/check.h"
#include "core/container/array.h"
#include "core/log/log.h"
#include "core/memory/alloc.h"

extern input_binding key_bindings[K_LAST + 1];
extern input_binding mouse_button_bindings[MB_LAST + 1];
extern axis_binding mouse_position_axis_bindings[2];
extern axis_binding key_axis_bindings[K_LAST + 1];
extern axis_binding mouse_button_axis_bindings[MB_LAST + 1];

void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_REPEAT || native_to_dfgame_key[key] == K_INVALID)
        return;
    key_bindings[native_to_dfgame_key[key]].active = action == GLFW_PRESS;
    key_axis_bindings[native_to_dfgame_key[key]].set = action == GLFW_PRESS;
}
void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_REPEAT || native_to_dfgame_mouse_button[button] == MB_INVALID)
        return;
    mouse_button_bindings[native_to_dfgame_mouse_button[button]].active = action == GLFW_PRESS;
    mouse_button_axis_bindings[native_to_dfgame_mouse_button[button]].set = action == GLFW_PRESS;
}
void input_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    mouse_button_bindings[MB_SCROLL_RIGHT].active = xoffset > 0;
    mouse_button_bindings[MB_SCROLL_LEFT].active = xoffset < 0;
    mouse_button_bindings[MB_SCROLL_DOWN].active = yoffset > 0;
    mouse_button_bindings[MB_SCROLL_UP].active = yoffset < 0;

    mouse_button_axis_bindings[MB_SCROLL_RIGHT].offset = xoffset;
    mouse_button_axis_bindings[MB_SCROLL_RIGHT].set = xoffset > 0;
    mouse_button_axis_bindings[MB_SCROLL_LEFT].offset = xoffset;
    mouse_button_axis_bindings[MB_SCROLL_LEFT].set = xoffset < 0;
    mouse_button_axis_bindings[MB_SCROLL_DOWN].offset = yoffset;
    mouse_button_axis_bindings[MB_SCROLL_DOWN].set = yoffset > 0;
    mouse_button_axis_bindings[MB_SCROLL_UP].offset = yoffset;
    mouse_button_axis_bindings[MB_SCROLL_UP].set = yoffset < 0;
}
void input_mouse_position_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(xoffset != 0) {
        if(mouse_position_axis_bindings[0].value != 0)
            mouse_position_axis_bindings[0].offset = mouse_position_axis_bindings[0].value - xoffset;
        mouse_position_axis_bindings[0].value = xoffset;
        mouse_position_axis_bindings[0].set = true;
    }
    if(yoffset != 0) {
        if(mouse_position_axis_bindings[1].value != 0)
            mouse_position_axis_bindings[1].offset = mouse_position_axis_bindings[1].value - yoffset;
        mouse_position_axis_bindings[1].value = yoffset;
        mouse_position_axis_bindings[1].set = true;
    }
}
