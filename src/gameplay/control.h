#ifndef DF_GAMEPLAY_CONTROL
#define DF_GAMEPLAY_CONTROL
#include "types.h"

typedef int16 action_id;
typedef int16 axis_id;

event(void, action_event, action_id id)

void update_controls();

action_id create_action(action_event* event);
void activate_action(action_id id);
bool action_is_active(action_id id);

axis_id create_axis(float limit);
void set_axis_value(axis_id id, float value);
float get_axis_value(axis_id id);

#endif
