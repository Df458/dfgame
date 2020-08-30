// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "application/input.h"
#include "application/input.priv.h"
#include "core/container/array.h"
#include "core/check.h"
#include "core/log/log.h"
#include "core/memory/alloc.h"

input_binding key_bindings[K_LAST + 1] = {0};
input_binding mouse_button_bindings[MB_LAST + 1] = {0};

axis_binding mouse_position_axis_bindings[2] = {0};
axis_binding key_axis_bindings[K_LAST + 1] = {0};
axis_binding mouse_button_axis_bindings[MB_LAST + 1] = {0};

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

axis_id input_add_mouse_position_axis(bool vertical, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_mouse_position_axis(vertical, id, mod);

    return id;
}

axis_id input_add_key_axis(key_id key, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_key_axis(key, id, mod);

    return id;
}
axis_id input_add_key_axis_full(key_id key_minus, key_id key_plus, float limit, float mod, bool digital) {
    axis_id axis = input_add_key_axis (key_plus, limit, mod, digital);
    input_bind_key_axis (key_minus, axis, -mod);

    return axis;
}
axis_id input_add_mouse_button_axis(mouse_button_id button, float limit, float mod, bool digital) {
    axis_id id = create_axis(limit, digital);
    input_bind_mouse_button_axis(button, id, mod);

    return id;
}
axis_id input_add_mouse_button_axis_full(mouse_button_id button_minus, mouse_button_id button_plus, float limit, float mod, bool digital) {
    axis_id axis = input_add_mouse_button_axis (button_plus, limit, mod, digital);
    input_bind_mouse_button_axis (button_minus, axis, -mod);

    return axis;
}

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

void axis_value_foreach(axis* a, float value) {
    set_axis_value(a->id, value * a->mod, value == 1);
}

void update_input() {
    update_controls();
    for(int i = 0; i < K_LAST; ++i) {
        if(key_bindings[i].actions != NULL && (key_bindings[i].active || key_bindings[i].triggered)) {
            key_bindings[i].triggered = false;
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
        if(mouse_button_bindings[i].actions != NULL && (mouse_button_bindings[i].active || mouse_button_bindings[i].triggered)) {
            mouse_button_bindings[i].triggered = false;
            array_foreach(mouse_button_bindings[i].actions, a) {
                activate_action(*(action_id*)a.data);
            }
        }
        if(mouse_button_axis_bindings[i].axes != NULL && mouse_button_axis_bindings[i].set) {
            array_foreach(mouse_button_axis_bindings[i].axes, it) {
                axis_value_foreach(it.data, 1.0f);
            }
        }
    }

    for(int i = MB_SCROLL_UP; i < MB_SCROLL_RIGHT; ++i) {
        mouse_button_axis_bindings[i].set = false;
    }

    if(mouse_position_axis_bindings[0].axes != NULL) {
        array_foreach(mouse_position_axis_bindings[0].axes, it) {
            axis_value_foreach(it.data, mouse_position_axis_bindings[0].offset);
        }
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
void clear_input_bindings() {
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
