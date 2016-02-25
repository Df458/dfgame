#ifndef INPUT_H
#define INPUT_H
#include <inttypes.h>
#include <stdbool.h>

// TODO: Update this to be more generic

uint8_t get_directional_input();
bool get_action_input();
bool get_confirm_input();
bool get_cancel_input();
bool get_menu_input();
bool get_quit_input();

void update_input_states();

void set_up_state(bool state);
void set_down_state(bool state);
void set_left_state(bool state);
void set_right_state(bool state);
void set_action_state(bool state);
void set_confirm_state(bool state);
void set_cancel_state(bool state);
void set_menu_state(bool state);
void set_quit_state(bool state);

#endif
