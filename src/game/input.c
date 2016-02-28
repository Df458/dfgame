#include "input.h"
#include "util.h"
#include <math.h>

// Private structs
struct axisState
{
    float x;
    float y;
};

// Private data
// TODO: Move to structs
static uint16_t input_down_counts[MAX_INPUTS] = { 0 };
static uint8_t input_states[MAX_INPUTS] = { 0 };

// Set directly
static float axis_x_analog_value[MAX_INPUTS] = { 0 };
static float axis_y_analog_value[MAX_INPUTS] = { 0 };

static int16_t axis_x_digital_counts[MAX_INPUTS] = { 0 };
static int16_t axis_y_digital_counts[MAX_INPUTS] = { 0 };

// Counts with interpolation
static int16_t axis_x_emulated_counts[MAX_INPUTS] = { 0 };
static int16_t axis_y_emulated_counts[MAX_INPUTS] = { 0 };
static float axis_x_emulated_value[MAX_INPUTS] = { 0 };
static float axis_y_emulated_value[MAX_INPUTS] = { 0 };

static struct axisState axis_states[MAX_INPUTS] = { 0 };

// FIXME: This might eat inputs
// TODO: Support axes
void update_input_states(float delta)
{
    for(int i = 0; i < MAX_INPUTS; ++i) {
        if(input_states[i] == 2)
            input_states[i] = 1;
        switch(input_states[i]) {
            case 2:
                input_states[i] = 1;
                break;
            case 3:
                input_states[i] = 0;
                break;
        }

        float clamped_x_value = 0;
        if(axis_x_emulated_counts[i] > 0)
            clamped_x_value = 1;
        else if(axis_x_emulated_counts[i] < 0)
            clamped_x_value = -1;

        float clamped_y_value = 0;
        if(axis_y_emulated_counts[i] > 0)
            clamped_y_value = 1;
        else if(axis_y_emulated_counts[i] < 0)
            clamped_y_value = -1;

        if(axis_x_emulated_value[i] < clamped_x_value)
            axis_x_emulated_value[i] += delta * 5;
        else if(axis_x_emulated_value[i] > clamped_x_value)
            axis_x_emulated_value[i] -= delta * 5;
        if(fabsf(axis_x_emulated_value[i] - clamped_x_value) < 0.1f)
            axis_x_emulated_value[i] = clamped_x_value;

        if(axis_y_emulated_value[i] < clamped_y_value)
            axis_y_emulated_value[i] += delta * 5;
        else if(axis_y_emulated_value[i] > clamped_y_value)
            axis_y_emulated_value[i] -= delta * 5;
        if(fabsf(axis_y_emulated_value[i] - clamped_y_value) < 0.1f)
            axis_y_emulated_value[i] = clamped_y_value;

        float axis_x_digital_value = axis_x_digital_counts[i];
        if(axis_x_digital_value > 1)
            axis_x_digital_value = 1;
        else if(axis_x_digital_value < -1)
            axis_x_digital_value = -1;
        float axis_y_digital_value = axis_y_digital_counts[i];
        if(axis_y_digital_value > 1)
            axis_y_digital_value = 1;
        else if(axis_y_digital_value < -1)
            axis_y_digital_value = -1;

        axis_states[i].x = axis_x_analog_value[i] + axis_x_emulated_value[i] + axis_x_digital_value;
        if(axis_states[i].x < -1)
            axis_states[i].x = -1;
        else if(axis_states[i].x > 1)
            axis_states[i].x = 1;
        axis_states[i].y = axis_y_analog_value[i] + axis_y_emulated_value[i] + axis_y_digital_value;
        if(axis_states[i].y < -1)
            axis_states[i].y = -1;
        else if(axis_states[i].y > 1)
            axis_states[i].y = 1;
    }
}

uint8_t get_input_state(uint8_t id)
{
    return input_states[id];
}

void set_input_state(uint8_t id, bool down)
{
    if(down) {
        if(input_down_counts[id] == 0)
            input_states[id] = 2;
        input_down_counts[id]++;
    } else {
        input_down_counts[id]--;
        if(input_down_counts[id] <= 0) {
            input_down_counts[id] = 0;
            input_states[id] = 3;
        }
    }
}

// FIXME: Emulation could potentially desync depending on when it's added
void set_axis(uint8_t id, bool isx, float value, bool analog, bool emulated)
{
    if(analog) {
        if(isx)
            axis_x_analog_value[id] = value;
        else
            axis_y_analog_value[id] = value;
    } else {
        if(!emulated) {
            if(isx)
                axis_x_digital_counts[id] += value;
            else
                axis_y_digital_counts[id] += value;
        } else {
            if(isx)
                axis_x_emulated_counts[id] += value > 0 ? 1 : -1;
            else
                axis_y_emulated_counts[id] += value > 0 ? 1 : -1;
        }
    }
}

void get_axis(uint8_t id, float* x, float* y)
{
    *x = axis_states[id].x;
    *y = axis_states[id].y;
}

float get_axis_x(uint8_t id)
{
    return axis_states[id].x;
}

float get_axis_y(uint8_t id)
{
    return axis_states[id].y;
}
