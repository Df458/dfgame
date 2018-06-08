#ifndef DF_GAMEPLAY_CONTROL
#define DF_GAMEPLAY_CONTROL
#include "core/types.h"

typedef int16 action_id;
typedef int16 axis_id;

event(action_event, action_id id)

void update_controls();

action_id create_action(action_event* event);
void activate_action(action_id id);
bool action_is_active(action_id id);
bool action_is_triggered(action_id id);
void set_action_event(action_id id, action_event* event);

axis_id create_axis(float limit, bool digital);
void set_axis_value(axis_id id, float value, bool digital);
float get_axis_value(axis_id id);

#endif
