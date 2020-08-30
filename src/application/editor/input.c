// Log category, used to filter logs
#define LOG_CATEGORY "Application"

#include "gdk_input.h"
#include "gdk_keymap.h"

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

bool on_button_press(GdkEventButton* event) {
    uint32 mb = convert_mouse_button(event->button);
    if(mb != MB_INVALID) {
        mouse_button_bindings[mb].active = true;
        mouse_button_bindings[mb].triggered = true;
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
        key_bindings[key].triggered = true;
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
