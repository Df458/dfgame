#include "util.h"
#include "vector.h"

vec2* create_vec2_data(float x, float y)
{
    vec2* vec = salloc(sizeof(vec2));
    vec->data[0] = x;
    vec->data[1] = y;

    return vec;
}

void destroy_vec2_full(vec2* vec)
{
    sfree(vec);
}
