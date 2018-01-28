// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "gdk_input.h"

#include "gdk_keymap.h"

#include "check.h"
#include "container/array.h"
#include "log/log.h"
#include "memory/alloc.h"

typedef struct input_binding {
    uarray actions;
    bool active;
} input_binding;
typedef struct axis {
    axis_id id;
    float   mod;
} axis;
typedef struct axis_binding {
    uarray axes;
    float value;
    float offset;
    bool set;
} axis_binding;

static input_binding key_bindings[K_LAST + 1] = {0};
static input_binding mouse_button_bindings[MB_LAST + 1] = {0};

static axis_binding mouse_position_axis_bindings[2] = {0};
static axis_binding key_axis_bindings[K_LAST + 1] = {0};
static axis_binding mouse_button_axis_bindings[MB_LAST + 1] = {0};

double __application_editor_mouse_x, __application_editor_mouse_y = 0;

void input_bind_key_action(key_id key, action_id id) {
    check_return(key > K_INVALID && key <= K_LAST, "Can't bind key: Index is out of range", );

    if(!key_bindings[key].actions)
        key_bindings[key].actions = uarray_new(1);

    array_copyadd_simple(key_bindings[key].actions, id);
}
void input_bind_mouse_button_action(mouse_button_id button, action_id id) {
    check_return(button <= MB_LAST, "Can't bind mouse button: Index is out of range", )

    if(!mouse_button_bindings[button].actions)
        mouse_button_bindings[button].actions = uarray_new(1);
    array_copyadd_simple(mouse_button_bindings[button].actions, id);
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
        key_axis_bindings[key].axes = uarray_new(1);
    axis* ax = salloc(sizeof(axis));
    ax->id = id;
    ax->mod = mod;
    array_add(key_axis_bindings[key].axes, ax);
}
void input_bind_mouse_button_axis(mouse_button_id button, axis_id id, float mod) {
    if(!mouse_button_axis_bindings[button].axes)
        mouse_button_axis_bindings[button].axes = uarray_new(1);
    axis* ax = salloc(sizeof(axis));
    ax->id = id;
    ax->mod = mod;
    array_add(mouse_button_axis_bindings[button].axes, ax);
}
void input_bind_mouse_position_axis(bool vertical, axis_id id, float mod) {
    uint8 index = vertical ? 1 : 0;

    if(!mouse_position_axis_bindings[index].axes)
        mouse_position_axis_bindings[index].axes = uarray_new(1);
    axis* ax = salloc(sizeof(axis));
    ax->id = id;
    ax->mod = mod;
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

uint32 convert_key(uint32 keyval) {
    for(int i = 0; i <= K_LAST; ++i)
        if(dfgame_to_native_key[i] == keyval)
            return i;

    return K_INVALID;
}
uint32 convert_mouse_button(uint32 button) {
    for(int i = 0; i <= MB_LAST; ++i)
        if(dfgame_to_native_mouse_button[i] == button)
            return i;

    return MB_INVALID;
}

iter_result action_active_foreach(void* iter_data, void* user) {
    activate_action(*(action_id*)iter_data);

    return iter_continue;
}
iter_result axis_value_foreach(void* iter_data, void* user) {
    float value = 1.0f;
    if(user)
        value = *(float*)user;
    axis* a = (axis*)iter_data;
    set_axis_value(a->id, value * a->mod, user == NULL);

    return iter_continue;
}

bool on_button_press(GdkEventButton* event) {
    uint32 mb = convert_mouse_button(event->button);
    if(mb != MB_INVALID) {
        mouse_button_bindings[mb].active = true;
        mouse_button_axis_bindings[mb].set = true;
    }

    return true;
}
bool on_button_release(GdkEventButton* event) {
    uint32 mb = convert_mouse_button(event->button);
    if(mb != MB_INVALID) {
        mouse_button_bindings[mb].active = false;
        mouse_button_axis_bindings[mb].set = false;
    }

    return true;
}
bool on_key_press(GdkEventKey* event) {
    uint32 key = convert_key(event->keyval);
    if(key != K_INVALID) {
        key_bindings[key].active = true;
        key_axis_bindings[key].set = true;
    }

    return true;
}
bool on_key_release(GdkEventKey* event) {
    uint32 key = convert_key(event->keyval);
    if(key != K_INVALID) {
        key_bindings[key].active = false;
        key_axis_bindings[key].set = false;
    }

    return true;
}
bool on_mouse_motion(GdkEventMotion* event) {

    __application_editor_mouse_x = event->x;
    __application_editor_mouse_y = event->y;
    if(event->axes[0] != 0) {
        if(mouse_position_axis_bindings[0].value != 0)
            mouse_position_axis_bindings[0].offset = mouse_position_axis_bindings[0].value - event->axes[0];
        mouse_position_axis_bindings[0].value = event->axes[0];
        mouse_position_axis_bindings[0].set = true;
    }
    if(event->axes[1] != 0) {
        if(mouse_position_axis_bindings[1].value != 0)
            mouse_position_axis_bindings[1].offset = mouse_position_axis_bindings[1].value - event->axes[1];
        mouse_position_axis_bindings[1].value = event->axes[1];
        mouse_position_axis_bindings[1].set = true;
    }

    return true;
}
bool on_scroll(GdkEventScroll* event) {
    mouse_button_bindings[MB_SCROLL_RIGHT].active = event->delta_x > 0;
    mouse_button_bindings[MB_SCROLL_LEFT].active = event->delta_x < 0;
    mouse_button_bindings[MB_SCROLL_DOWN].active = event->delta_y > 0;
    mouse_button_bindings[MB_SCROLL_UP].active = event->delta_y < 0;

    mouse_button_axis_bindings[MB_SCROLL_RIGHT].offset = event->delta_x;
    mouse_button_axis_bindings[MB_SCROLL_RIGHT].set = event->delta_x > 0;
    mouse_button_axis_bindings[MB_SCROLL_LEFT].offset = event->delta_x;
    mouse_button_axis_bindings[MB_SCROLL_LEFT].set = event->delta_x < 0;
    mouse_button_axis_bindings[MB_SCROLL_DOWN].offset = event->delta_y;
    mouse_button_axis_bindings[MB_SCROLL_DOWN].set = event->delta_y > 0;
    mouse_button_axis_bindings[MB_SCROLL_UP].offset = event->delta_y;
    mouse_button_axis_bindings[MB_SCROLL_UP].set = event->delta_y < 0;

    return true;
}

void update_input() {
    update_controls();
    for(int i = 0; i < K_LAST; ++i) {
        if(key_bindings[i].actions != NULL && key_bindings[i].active)
            array_foreach(key_bindings[i].actions, action_active_foreach, NULL);
        if(key_axis_bindings[i].axes != NULL && key_axis_bindings[i].set)
            array_foreach(key_axis_bindings[i].axes, axis_value_foreach, NULL);
    }
    for(int i = 0; i < MB_LAST; ++i) {
        if(mouse_button_bindings[i].actions != NULL && mouse_button_bindings[i].active)
            array_foreach(mouse_button_bindings[i].actions, action_active_foreach, NULL);
        if(mouse_button_axis_bindings[i].axes != NULL && mouse_button_axis_bindings[i].set)
            array_foreach(mouse_button_axis_bindings[i].axes, axis_value_foreach, NULL);
    }

    for(int i = MB_SCROLL_UP; i < MB_SCROLL_RIGHT; ++i) {
        mouse_button_axis_bindings[i].set = false;
    }

    if(mouse_position_axis_bindings[0].axes != NULL)
        array_foreach(mouse_position_axis_bindings[0].axes, axis_value_foreach, &mouse_position_axis_bindings[0].offset);
    if(mouse_position_axis_bindings[1].axes != NULL)
        array_foreach(mouse_position_axis_bindings[1].axes, axis_value_foreach, &mouse_position_axis_bindings[1].offset);
    mouse_position_axis_bindings[0].set = false;
    mouse_position_axis_bindings[0].offset = 0;
    mouse_position_axis_bindings[1].set = false;
    mouse_position_axis_bindings[1].offset = 0;
}
