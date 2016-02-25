#include "input.h"

bool up_state = false;
bool down_state = false;
bool left_state = false;
bool right_state = false;
bool action_state = false;
bool confirm_state = false;
bool cancel_state = false;
bool menu_state = false;
bool quit_state = false;

bool y_input_prio = false;

uint8_t get_directional_input()
{
    int8_t x = 0;
    int8_t y = 0;

    if(left_state > 0)
        --x;
    if(right_state > 0)
        ++x;
    if(up_state > 0)
        --y;
    if(down_state > 0)
        ++y;

    if (x && y) {
        if(y_input_prio)
            x = 0;
        else
            y = 0;
    }
    if(x) {
        return x > 0 ? 2 : 1;
    } else if(y) {
        return y > 0 ? 4 : 3;
    }

    return 0;
}

bool get_action_input()
{
    return action_state;
}

bool get_confirm_input()
{
    return confirm_state;
}

bool get_cancel_input()
{
    return cancel_state;
}

bool get_menu_input()
{
    return menu_state;
}

bool get_quit_input()
{
    return quit_state;
}

void update_input_states()
{
    action_state = 0;
    confirm_state = 0;
    cancel_state = 0;
}

void set_up_state(bool state) { up_state = state; y_input_prio = true; }
void set_down_state(bool state) { down_state = state; y_input_prio = true; }
void set_left_state(bool state) { left_state = state; y_input_prio = false; }
void set_right_state(bool state) { right_state = state; y_input_prio = false; }
void set_action_state(bool state) { action_state = state; }
void set_confirm_state(bool state) { confirm_state = state; }
void set_cancel_state(bool state) { cancel_state = state; }
void set_menu_state(bool state) { menu_state = state; }
void set_quit_state(bool state) { quit_state = state; }
