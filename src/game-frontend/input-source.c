#include "input-source.h"
#include "input.h"
#include "util.h"

#include <stdlib.h>

// Private structs
struct actionPair
{
    int mods;
    uint8_t action_id;
};

struct axisPair
{
    int mods;
    uint8_t axis_id;
    bool emulated;
    bool isx;
    float value;
};

struct keyInput
{
    uint16_t action_count;
    uint16_t axis_count;

    struct actionPair* action_pairs;
    struct axisPair*   axis_pairs;
};

// Private data
static struct keyInput key_inputs[GLFW_KEY_LAST];

void keyCodeCallback(GLFWwindow* window, int key_code, int scancode, int action, int mod)
{
    if(action == GLFW_REPEAT)
        return;

    for(int i = 0; i < key_inputs[key_code].action_count; ++i) {
        if(key_inputs[key_code].action_pairs[i].mods == mod) {
            set_input_state(key_inputs[key_code].action_pairs[i].action_id, action == GLFW_PRESS);
        }
    }

    for(int i = 0; i < key_inputs[key_code].axis_count; ++i) {
        if(key_inputs[key_code].axis_pairs[i].mods == mod) {
            // FIXME: On release, digital axes are reversed
            if(action == GLFW_PRESS)
                set_axis(key_inputs[key_code].axis_pairs[i].axis_id, key_inputs[key_code].axis_pairs[i].isx, key_inputs[key_code].axis_pairs[i].value, false, key_inputs[key_code].axis_pairs[i].emulated);
            else {
                set_axis(key_inputs[key_code].axis_pairs[i].axis_id, key_inputs[key_code].axis_pairs[i].isx, key_inputs[key_code].axis_pairs[i].value * -1, false, key_inputs[key_code].axis_pairs[i].emulated);
            }
        }
    }
}

bool init_input(GLFWwindow* win)
{
    nulltest(win);

    glfwSetKeyCallback(win, keyCodeCallback);

    return true;
}

bool map_key_to_action(int key, int mods, uint8_t action)
{
    key_inputs[key].action_pairs = realloc(key_inputs[key].action_pairs, (key_inputs[key].action_count + 1) * sizeof(struct actionPair));
    key_inputs[key].action_pairs[key_inputs[key].action_count].mods = mods;
    key_inputs[key].action_pairs[key_inputs[key].action_count].action_id = action;
    key_inputs[key].action_count++;
    return true;
}

bool map_key_to_axis(int key, int mods, uint8_t axis, bool isx, bool positive, bool emulated)
{
    key_inputs[key].axis_pairs = realloc(key_inputs[key].axis_pairs, (key_inputs[key].axis_count + 1) * sizeof(struct axisPair));
    key_inputs[key].axis_pairs[key_inputs[key].axis_count].mods = mods;
    key_inputs[key].axis_pairs[key_inputs[key].axis_count].axis_id = axis;
    key_inputs[key].axis_pairs[key_inputs[key].axis_count].emulated = emulated;
    key_inputs[key].axis_pairs[key_inputs[key].axis_count].isx = isx;
    key_inputs[key].axis_pairs[key_inputs[key].axis_count].value = positive ? 1 : -1;
    key_inputs[key].axis_count++;
    return true;
}

bool unmap_key(int key, int mods)
{
    stub(false);
}

bool map_mouse_button_to_action(int button, int mods, uint8_t action)
{
    stub(false);
}

bool map_mouse_button_to_axis(int button, int mods, uint8_t action, bool emulated)
{
    stub(false);
}

bool unmap_mouse_button(int button, int mods)
{
    stub(false);
}
