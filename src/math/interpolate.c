// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "interpolate.h"

#include "log/log.h"
#include "mathutil.h"
#include "quat.h"
#include "vector.h"

int int_lerp(int start, int end, float position) {
    return start + (end - start) * clamp(position, 0, 1);
}
float float_lerp(float start, float end, float position) {
    return start + (end - start) * clamp(position, 0, 1);
}
vec2 vec2_lerp(vec2 start, vec2 end, float position) {
    vec2 v = start;
    v.x += (end.x - start.x) * clamp(position, 0, 1);
    v.y += (end.y - start.y) * clamp(position, 0, 1);

    return v;
}
vec3 vec3_lerp(vec3 start, vec3 end, float position) {
    vec3 v = start;
    v.x += (end.x - start.x) * clamp(position, 0, 1);
    v.y += (end.y - start.y) * clamp(position, 0, 1);
    v.z += (end.z - start.z) * clamp(position, 0, 1);

    return v;
}
vec4 vec4_lerp(vec4 start, vec4 end, float position) {
    vec4 v = start;
    v.x += (end.x - start.x) * clamp(position, 0, 1);
    v.y += (end.y - start.y) * clamp(position, 0, 1);
    v.z += (end.z - start.z) * clamp(position, 0, 1);
    v.w += (end.w - start.w) * clamp(position, 0, 1);

    return v;
}

quat slerp(quat start, quat end, float position) {
    stub(start);
}
