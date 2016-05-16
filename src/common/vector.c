#include "util.h"
#include "vector.h"

vec2 create_vec2_data(float x, float y)
{
    vec2 vec;
    vec.data[0] = x;
    vec.data[1] = y;

    return vec;
}

/* void destroy_vec2_full(vec2* vec) */
/* { */
/*     sfree(vec); */
/* } */

vec3 create_vec3_data(float x, float y, float z)
{
    vec3 vec;
    vec.data[0] = x;
    vec.data[1] = y;
    vec.data[2] = z;

    return vec;
}

/* void destroy_vec3_full(vec3* vec) */
/* { */
/*     sfree(vec); */
/* } */

vec4 create_vec4_data(float x, float y, float z, float w)
{
    vec4 vec;
    vec.data[0] = x;
    vec.data[1] = y;
    vec.data[2] = z;
    vec.data[3] = w;

    return vec;
}

/* void destroy_vec4_full(vec4* vec) */
/* { */
/*     sfree(vec); */
/* } */
