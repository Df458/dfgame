// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "glfw_input.h"

#include "glfw_keymap.h"

#include "core/check.h"
#include "core/container/array.h"
#include "core/log/log.h"
#include "core/memory/alloc.h"

typedef struct input_binding {
    array actions;
    bool active;
} input_binding;
typedef struct axis {
    axis_id id;
    float   mod;
} axis;
typedef struct axis_binding {
    array axes;
    float value;
    float offset;
    bool set;
} axis_binding;

static input_binding key_bindings[K_LAST + 1] = {0};
static input_binding mouse_button_bindings[MB_LAST + 1] = {0};

static axis_binding mouse_position_axis_bindings[2] = {0};
static axis_binding key_axis_bindings[K_LAST + 1] = {0};
static axis_binding mouse_button_axis_bindings[MB_LAST + 1] = {0};

void input_bind_key_action(key_id key, action_id id) {
    check_return(key > K_INVALID && key <= K_LAST, "Can't bind key: Index is out of range", );

    if(!key_bindings[key].actions)
        key_bindings[key].actions = array_mnew(action_id, 1);

    array_add(key_bindings[key].actions, id);
}
void input_bind_mouse_button_action(mouse_button_id button, action_id id) {
    check_return(button <= MB_LAST, "Can't bind mouse button: Index is out of range", )

    if(!mouse_button_bindings[button].actions)
        mouse_button_bindings[button].actions = array_mnew(action_id, 1);
    array_add(mouse_button_bindings[button].actions, id);
}
action_id input_add_key_action(key_id key, action_event* event) {
    action_id id = create_action(event);
    input_bind_key_action(key, id);

    return id;
}
action_id input_add_mouse_button_action(mouse_button_id button, action_event* event) {
    action_id id = create_action(event);
    input_bind_mouse_button_action(button, id);

    return id;
}

void input_bind_key_axis(key_id key, axis_id id, float mod) {
    if(!key_axis_bindings[key].axes)
        key_axis_bindings[key].axes = array_mnew(axis, 1);
    axis ax = {
        .id = id,
        .mod = mod
    };
    array_add(key_axis_bindings[key].axes, ax);
}
void input_bind_mouse_button_axis(mouse_button_id button, axis_id id, float mod) {
    if(!mouse_button_axis_bindings[button].axes)
        mouse_button_axis_bindings[button].axes = array_mnew(axis, 1);
    axis ax = {
        .id = id,
        .mod = mod
    };
    array_add(mouse_button_axis_bindings[button].axes, ax);
}
void input_bind_mouse_position_axis(bool vertical, axis_id id, float mod) {
    uint8 index = vertical ? 1 : 0;

    if(!mouse_position_axis_bindings[index].axes)
        mouse_position_axis_bindings[index].axes = array_mnew(axis, 1);
    axis ax = {
        .id = id,
        .mod = mod
    };
    array_add(mouse_position_axis_bindings[index].axes, ax);
}
axis_id input_add_key_axis(key_id key, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_key_axis(key, id, mod);

    return id;
}
axis_id input_add_mouse_button_axis(mouse_button_id button, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_mouse_button_axis(button, id, mod);

    return id;
}
axis_id input_add_mouse_position_axis(bool vertical, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_mouse_position_axis(vertical, id, mod);

    return id;
}

void axis_value_foreach(axis* a, float value) {
    set_axis_value(a->id, value * a->mod, value == 1.0f);
}

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

void update_input() {
    update_controls();
    for(int i = 0; i < K_LAST; ++i) {
        if(key_bindings[i].actions != NULL && key_bindings[i].active) {
            array_foreach(key_bindings[i].actions, a) {
                activate_action(*(action_id*)a.data);
            }
        }
        if(key_axis_bindings[i].axes != NULL && key_axis_bindings[i].set) {
            array_foreach(key_axis_bindings[i].axes, it) {
                axis_value_foreach(it.data, 1.0f);
            }
        }
    }
    for(int i = 0; i < MB_LAST; ++i) {
        if(mouse_button_bindings[i].actions != NULL && mouse_button_bindings[i].active) {
            array_foreach(mouse_button_bindings[i].actions, a) {
                activate_action(*(action_id*)a.data);
            }
        }
        if(mouse_button_axis_bindings[i].axes != NULL && mouse_button_axis_bindings[i].set)
            array_foreach(mouse_button_axis_bindings[i].axes, it) {
                axis_value_foreach(it.data, 1.0f);
            }
    }

    for(int i = MB_SCROLL_UP; i < MB_SCROLL_RIGHT; ++i) {
        mouse_button_axis_bindings[i].set = false;
    }

    if(mouse_position_axis_bindings[0].axes != NULL)
        array_foreach(mouse_position_axis_bindings[0].axes, it) {
            axis_value_foreach(it.data, mouse_position_axis_bindings[0].offset);
        }
    if(mouse_position_axis_bindings[1].axes != NULL) {
        array_foreach(mouse_position_axis_bindings[1].axes, it) {
            axis_value_foreach(it.data, mouse_position_axis_bindings[1].offset);
        }
    }
    mouse_position_axis_bindings[0].set = false;
    mouse_position_axis_bindings[0].offset = 0;
    mouse_position_axis_bindings[1].set = false;
    mouse_position_axis_bindings[1].offset = 0;
}

void clear_input_bindings()
{
    for(int i = 0; i < K_LAST; ++i) {
        if(key_bindings[i].actions != NULL)
            array_free(key_bindings[i].actions);
        if(key_axis_bindings[i].axes != NULL)
            array_free(key_axis_bindings[i].axes);
    }
    for(int i = 0; i < MB_LAST; ++i) {
        if(mouse_button_bindings[i].actions != NULL)
            array_free(mouse_button_bindings[i].actions);
        if(mouse_button_axis_bindings[i].axes != NULL)
            array_free(mouse_button_axis_bindings[i].axes);
    }

    if(mouse_position_axis_bindings[0].axes != NULL)
        array_free(mouse_position_axis_bindings[0].axes);
    if(mouse_position_axis_bindings[1].axes != NULL)
        array_free(mouse_position_axis_bindings[1].axes);
}
