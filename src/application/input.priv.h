#ifndef DF_APPLICATION_INPUT_PRIV
#define DF_APPLICATION_INPUT_PRIV
#include "core/container/array.h"

typedef struct input_binding {
    array actions;
    bool active;
    bool triggered;
} input_binding;

typedef struct axis {
    axis_id id;
    float   mod;
} axis;

typedef struct axis_binding {
    array axes;
    float value;
    float offset;
    bool set;
} axis_binding;
#endif
