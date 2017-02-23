// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "vector.h"

#include "log/log.h"
#include "quat.h"
#include <math.h>

// Returns the length of the vactor
// NOTE: When comparing two vectors, vec_len_squared may be preferrable as it's
// much faster.
float vec2_len(vec2 v) {
    return sqrt(vec2_len_squared(v));
}
float vec3_len(vec3 v) {
    return sqrt(vec3_len_squared(v));
}
float vec4_len(vec4 v) {
    return sqrt(vec4_len_squared(v));
}

// Returns the squared length of the vactor
float vec2_len_squared(vec2 v) {
    return pow(v.x, 2) + pow(v.y, 2);
}
float vec3_len_squared(vec3 v) {
    return pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
}
float vec4_len_squared(vec4 v) {
    return pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2);
}

// Normalizes the vector, making its length 1.
vec2 vec2_normalize(vec2 v) {
    float len = vec2_len(v);

    if(len == 0) {
        error("Attempting to normalize a vec2 of 0 length is undefined.");
        return (vec2){ 0 };
    }

    return (vec2){ .x = v.x / len, .y = v.y / len };
}
vec3 vec3_normalize(vec3 v) {
    float len = vec3_len(v);

    if(len == 0) {
        error("Attempting to normalize a vec3 of 0 length is undefined.");
        return (vec3){ 0 };
    }

    return (vec3){ .x = v.x / len, .y = v.y / len, .z = v.z / len };
}
vec4 vec4_normalize(vec4 v) {
    float len = vec4_len(v);

    if(len == 0) {
        error("Attempting to normalize a vec4 of 0 length is undefined.");
        return (vec4){ 0 };
    }

    return (vec4){ .x = v.x / len, .y = v.y / len, .z = v.z / len, .w = v.w / len };
}

// Adds 2 vectors together
vec2 vec2_add(vec2 v1, vec2 v2) {
    return (vec2){ .x = v1.x + v2.x, .y = v1.y + v2.y };
}
vec3 vec3_add(vec3 v1, vec3 v2) {
    return (vec3){ .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
}
vec4 vec4_add(vec4 v1, vec4 v2) {
    return (vec4){ .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, v1.w + v2.w };
}

// Subtracts v2 from v1
vec2 vec2_sub(vec2 v1, vec2 v2) {
    return (vec2) { .x = v1.x - v2.x, .y = v1.y - v2.y };
}
vec3 vec3_sub(vec3 v1, vec3 v2) {
    return (vec3) { .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z };
}
vec4 vec4_sub(vec4 v1, vec4 v2) {
    return (vec4) { .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z, .w = v1.w - v2.w };
}

// Multiplies v by the scalar x
vec2 vec2_mul(vec2 v, float f) {
    return (vec2) { .x = v.x * f, .y = v.y * f };
}
vec3 vec3_mul(vec3 v, float f) {
    return (vec3) { .x = v.x * f, .y = v.y * f, .z = v.z * f };
}
vec4 vec4_mul(vec4 v, float f) {
    return (vec4) { .x = v.x * f, .y = v.y * f, .z = v.z * f, .w = v.w * f };
}

// Returns the dot product between v1 and v2
float vec2_dot(vec2 v1, vec2 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}
float vec3_dot(vec3 v1, vec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
float vec4_dot(vec4 v1, vec4 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

// Returns the cross product of v1 and v2.
vec3 vec3_cross(vec3 v1, vec3 v2) {
    return (vec3){ .x = (v1.y * v2.z) - (v1.z * v2.y), .y = (v1.z * v2.x) - (v1.x * v2.z), .z = (v1.x * v2.y) - (v1.y * v2.x) };
}

vec2 vec2_rotate(vec2 v, float angle) {
    return (vec2) {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle)
    };
}
vec3 vec3_rotate(vec3 v, quat q) {
    stub(v);
}
vec4 vec4_rotate(vec4 v, quat q) {
    stub(v);
}
