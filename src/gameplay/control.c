// Log category, used to filter logs
#define LOG_CATEGORY "Gameplay"

#include "control.h"

#include "log/log.h"
#include "mathutil.h"
#include "memory/alloc.h"

typedef struct action {
    action_event* event;

    bool active;
    bool set;
} action;

typedef struct axis {
    float value;
    float limit;
    bool set;
} axis;

static action action_list[256];
static int16 action_count = 0;

static axis axis_list[256];
static int16 axis_count = 0;

void update_controls() {
    for(int i = 0; i < action_count; ++i) {
        if(!action_list[i].set)
            action_list[i].active = false;
        action_list[i].set = false;
    }
    for(int i = 0; i < axis_count; ++i) {
        if(!axis_list[i].set)
            axis_list[i].value = 0;
        axis_list[i].set = false;
    }
}

action_id create_action(action_event* event) {
    if(action_count >= 256) {
        error("Can't create action: Maximum number of actions created");
        return -1;
    }
    action_id id = action_count;
    bind_event(action_event, action_list[id].event, event);

    action_count++;

    return id;
}

void activate_action(action_id id) {
    if(id >= action_count) {
        error("Can't access action: Index is out of range");
    }

    if(!action_list[id].active)
        call_event(action_list[id].event, id);

    action_list[id].active = true;
    action_list[id].set    = true;
}

bool action_is_active(action_id id) {
    if(id >= action_count) {
        error("Can't access action: Index is out of range");
        return false;
    }

    return action_list[id].active;
}

axis_id create_axis(float limit) {
    if(axis_count >= 256) {
        error("Can't create axis: Maximum number of axes created");
        return -1;
    }
    axis_id id = axis_count;
    axis_list[id].limit = limit;

    axis_count++;

    return id;
}

void set_axis_value(axis_id id, float value) {
    if(id >= axis_count) {
        error("Can't access axis: Index is out of range");
    }

    axis_list[id].value  = value;
    axis_list[id].set    = true;
}

float get_axis_value(axis_id id) {
    if(id >= axis_count) {
        error("Can't access axis: Index is out of range");
        return false;
    }

    if(axis_list[id].limit != 0)
        return clamp(axis_list[id].value, -axis_list[id].limit, axis_list[id].limit);
    else
        return axis_list[id].value;
}
