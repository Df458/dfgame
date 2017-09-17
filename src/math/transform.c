// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "transform.h"

#include "log/log.h"
#include "memory/alloc.h"
#include "matrix.h"
#include "vector.h"

// Represents a transform, containing both separate components and the
// resulting transformation matrix.
typedef struct transform {
    vec3 position;
    quat orientation;
    vec3 scale;

    mat4 matrix;
}* transform;

// Recalculates the transform's matrix
static void recalculate_matrix(transform t) {
    mat4 s = mat4_scale(mat4_ident, t->scale);
    mat4 r = mat4_rotate(mat4_ident, t->orientation);
    t->matrix = mat4_mul(mat4_translate(mat4_ident, t->position), mat4_mul(r, s));
}

//-----------------------------------------------------------------------------

// Creates a new transform with default values
transform transform_new() {
    transform t = salloc(sizeof(struct transform));

    t->position = (vec3){0};
    t->orientation = quat_ident;
    t->scale = (vec3){ .x=1, .y=1, .z=1 };
    t->matrix = mat4_ident;

    return t;
}

// Sets or adds the transform's position
void transform_translate_2d(transform t, vec2 v, bool relative) {
    if(relative)
        t->position = vec3_add(t->position, (vec3){.xy=v});
    else
        t->position = (vec3){.xy=v};

    recalculate_matrix(t);
}
void transform_translate_3d(transform t, vec3 v, bool relative) {
    if(relative)
        t->position = vec3_add(t->position, v);
    else
        t->position = v;

    recalculate_matrix(t);
}

// Gets the position of the transform
vec3 transform_get_position(transform t) {
    return t->position;
}

// Rotates or sets the orientation of the transform
void transform_rotate_2d(transform t, float theta, bool relative) {
    transform_rotate_3d(t, (vec3){.data={0, 0, theta}}, relative);
}
void transform_rotate_3d(transform t, vec3 v, bool relative) {
    if(relative)
        t->orientation = quat_normalize(quat_mul(quat_normalize(euler_to_quat(v)), t->orientation));
    else
        t->orientation = quat_normalize(euler_to_quat(v));

    recalculate_matrix(t);
}
void transform_rotate_3d_quat(transform t, quat q, bool relative) {
    if(relative)
        t->orientation = quat_normalize(quat_mul(t->orientation, q));
    else
        t->orientation = quat_normalize(q);

    recalculate_matrix(t);
}

// Sets the orientation of t to face target
void transform_lookat(transform t, vec3 target, vec3 up) {
    /* t->orientation = quat_normalize(mat4_to_quat(mat4_lookat(t->position, target, up))); */
    transform_rotate_3d(t, mat4_to_euler(mat4_lookat(t->position, target, up)), false);
    /* info("w\n" mat4_printstr quat_printstr, mat4_printargs(mat4_lookat(t->position, target, up)), quat_printargs(t->orientation)); */
    /* recalculate_matrix(t); */
    /* t->matrix = mat4_translate(, vec3_mul(t->position, -1)); */
    /* mat4 s = mat4_scale(mat4_ident, t->scale); */
    /* t->matrix = mat4_mul(mat4_translate(mat4_ident, t->position), mat4_mul(mat4_lookat(t->position, target, up), s)); */
    /* info(mat4_printstr, mat4_printargs(t->matrix)); */
}

// Gets the orientation of the transform
quat transform_get_orientation(transform t) {
    return t->orientation;
}
vec3 transform_get_orientation_euler(transform t) {
    return quat_to_euler(t->orientation);
}
float transform_get_orientation_2d(transform t) {
    return quat_to_euler(t->orientation).z;
}

// Sets or adds the transform's scale
void transform_scale_1d(transform t, float v, bool relative) {
    if(relative)
        t->scale = vec3_mul(t->scale, v);
    else
        t->scale = (vec3){.x=v, .y=v, .z=v};

    recalculate_matrix(t);
}
void transform_scale_2d(transform t, vec2 v, bool relative) {
    transform_scale_3d(t, (vec3){.xy=v}, relative);
}
void transform_scale_3d(transform t, vec3 v, bool relative) {
    if(relative)
        t->scale = vec3_add(t->scale, v);
    else
        t->scale = v;

    recalculate_matrix(t);
}

// Gets the scale of the transform
vec3 transform_get_scale(transform t) {
    return t->scale;
}

mat4 transform_get_matrix(transform t) {
    return t->matrix;
}

// Frees the transform
void _transform_free(transform t) {
    sfree(t);
}
