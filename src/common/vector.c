#include "util.h"
#include "vector.h"

vec2 create_vec2_data(float x, float y)
{
    vec2 vec;
    vec.data[0] = x;
    vec.data[1] = y;

    return vec;
}

float vec2_dot(vec2 a, vec2 b)
{
    return (a.data[0]*b.data[0]) + (a.data[1]*b.data[1]);
}

vec3 create_vec3_data(float x, float y, float z)
{
    vec3 vec;
    vec.data[0] = x;
    vec.data[1] = y;
    vec.data[2] = z;

    return vec;
}

float vec3_dot(vec3 a, vec3 b)
{
    return (a.data[0]*b.data[0]) + (a.data[1]*b.data[1]) + (a.data[2]*b.data[2]);
}

vec4 create_vec4_data(float x, float y, float z, float w)
{
    vec4 vec;
    vec.data[0] = x;
    vec.data[1] = y;
    vec.data[2] = z;
    vec.data[3] = w;

    return vec;
}

float vec4_dot(vec4 a, vec4 b)
{
    return (a.data[0]*b.data[0]) + (a.data[1]*b.data[1]) + (a.data[2]*b.data[2]) + (a.data[3]*b.data[3]);
}
