#ifndef DF_INPUT_H
#define DF_INPUT_H
#include <inttypes.h>
#include <stdbool.h>

#define MAX_INPUTS 256

void update_input_states(float delta);

uint8_t get_input_state(uint8_t id);
void set_input_state(uint8_t id, bool down);
void set_axis(uint8_t id, bool isx, float value, bool analog, bool emulated);
void get_axis(uint8_t id, float* x, float* y);
float get_axis_x(uint8_t id);
float get_axis_y(uint8_t id);

#endif
