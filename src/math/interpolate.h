#ifndef DF_INTERPOLATE_H
#define DF_INTERPOLATE_H

#include "quat.hd"
#include "vector.hd"

#define lerp(s, e, p) _Generic(s,\
    int: int_lerp(s, e, p),\
    float: float_lerp(s, e, p),\
    vec2: vec2_lerp(s, e, p),\
    vec3: vec3_lerp(s, e, p),\
    vec4: vec4_lerp(s, e, p),\
)
int int_lerp(int start, int end, float position);
float float_lerp(float start, float end, float position);
vec2 vec2_lerp(vec2 start, vec2 end, float position);
vec3 vec3_lerp(vec3 start, vec3 end, float position);
vec4 vec4_lerp(vec4 start, vec4 end, float position);

quat slerp(quat start, quat end, float position);

#endif
