#include "util.h"
#include "vector.h"

float vec2_dot(vec2 a, vec2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

float vec3_dot(vec3 a, vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float vec4_dot(vec4 a, vec4 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}
