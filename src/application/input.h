#ifndef DF_APPLICATION_INPUT
#define DF_APPLICATION_INPUT

#include "gameplay/control.h"
#include "application/input_id.h"

// Create an action and bind a key
action_id input_add_key_action(key_id key, action_event* event);
// Create an action and bind a mouse button
action_id input_add_mouse_button_action(mouse_button_id button, action_event* event);

// Create an axis and bind a key
axis_id input_add_key_axis(key_id key, float limit, float mod, bool digital);
// Create an axis and bind two keys to opposite ends
axis_id input_add_key_axis_full(key_id key_minus, key_id key_plus, float limit, float mod, bool digital);
// Create an axis and bind a mouse button
axis_id input_add_mouse_button_axis(mouse_button_id button, float limit, float mod, bool digital);
// Create an axis and bind two mouse buttons to opposite ends
axis_id input_add_mouse_button_axis_full(mouse_button_id button_minus, mouse_button_id button_plus, float limit, float mod, bool digital);

// Create an axis and bind it to one of the axes of the mouse's position
axis_id input_add_mouse_position_axis(bool vertical, float limit, float mod, bool digital);

// Bind a key to an existing action
void input_bind_key_action(key_id key, action_id id);
// Bind a mouse button to an existing action
void input_bind_mouse_button_action(mouse_button_id button, action_id id);

// Bind a key to an existing axis
void input_bind_key_axis(key_id key, axis_id id, float mod);
// Bind a mouse button to an existing axis
void input_bind_mouse_button_axis(mouse_button_id button, axis_id id, float mod);

// Bind an axis of the mouse's position to an existing axis
void input_bind_mouse_position_axis(bool vertical, axis_id id, float mod);

// Update all input actions/axes with input since the last update
void update_input();

// Clear all existing input bindings
void clear_input_bindings();

#endif
