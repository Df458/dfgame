#ifndef DF_APPLICATION_INPUT
#define DF_APPLICATION_INPUT

#include "control.h"
#include "input_id.h"

void input_bind_key_action(key_id key, action_id id);
void input_bind_mouse_button_action(mouse_button_id button, action_id id);
action_id input_add_key_action(key_id key, action_event* event);
action_id input_add_mouse_button_action(mouse_button_id button, action_event* event);

void input_bind_key_axis(key_id key, axis_id id, float mod);
void input_bind_mouse_button_axis(mouse_button_id button, axis_id id, float mod);
void input_bind_mouse_position_axis(bool vertical, axis_id id, float mod);
axis_id input_add_key_axis(key_id key, float limit, float mod, bool digital);
axis_id input_add_mouse_button_axis(mouse_button_id button, float limit, float mod, bool digital);
axis_id input_add_mouse_position_axis(bool vertical, float limit, float mod, bool digital);

void update_input();

#endif
