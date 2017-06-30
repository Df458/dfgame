// Log category, used to filter logs
#define LOG_CATEGORY "Math"

#include "aabb.h"

// Gets the area of the aabb
float aabb_area_2d(aabb_2d box) {
    return box.dimensions.x * box.dimensions.y;
}
float aabb_volume_3d(aabb_3d box) {
    return box.dimensions.x * box.dimensions.y * box.dimensions.z;
}

// Gets whether or not the two aabbs intersect
bool aabb_intersects_2d(aabb_2d box1, aabb_2d box2) {
    return ((box1.position.x < box2.position.x + box2.dimensions.x && box1.position.x + box1.dimensions.x > box2.position.x) ||
           (box2.position.x < box1.position.x + box1.dimensions.x && box2.position.x + box2.dimensions.x > box1.position.x)) &&
           ((box1.position.y < box2.position.y + box2.dimensions.y && box1.position.y + box1.dimensions.y > box2.position.y) ||
           (box2.position.y < box1.position.y + box1.dimensions.y && box2.position.y + box2.dimensions.y > box1.position.y));
}
bool aabb_intersects_3d(aabb_3d box1, aabb_3d box2) {
    return ((box1.position.x < box2.position.x + box2.dimensions.x && box1.position.x + box1.dimensions.x > box2.position.x) ||
           (box2.position.x < box1.position.x + box1.dimensions.x && box2.position.x + box2.dimensions.x > box1.position.x)) &&
           ((box1.position.y < box2.position.y + box2.dimensions.y && box1.position.y + box1.dimensions.y > box2.position.y) ||
           (box2.position.y < box1.position.y + box1.dimensions.y && box2.position.y + box2.dimensions.y > box1.position.y)) &&
           ((box1.position.z < box2.position.z + box2.dimensions.z && box1.position.z + box1.dimensions.z > box2.position.z) ||
           (box2.position.z < box1.position.z + box1.dimensions.z && box2.position.z + box2.dimensions.z > box1.position.z));
}
bool aabb_intersects_point_2d(aabb_2d box, vec2 point) {
    return box.position.x < point.x && box.position.x + box.dimensions.x > point.x &&
           box.position.y < point.y && box.position.y + box.dimensions.y > point.y;
}
bool aabb_intersects_point_3d(aabb_3d box, vec3 point) {
    return box.position.x < point.x && box.position.x + box.dimensions.x > point.x &&
           box.position.y < point.y && box.position.y + box.dimensions.y > point.y &&
           box.position.z < point.z && box.position.z + box.dimensions.z > point.z;
}

// Returns a new aabb based on the overlap of the two given aabbs
aabb_2d aabb_get_intersection_2d(aabb_2d box1, aabb_2d box2) {
    if(!aabb_intersects_2d(box1, box2))
        return (aabb_2d){0};

    float xmin, xmax, ymin, ymax;
    xmin = box1.position.x < box2.position.x ? box1.position.x : box2.position.x;
    ymin = box1.position.y < box2.position.y ? box1.position.y : box2.position.y;

    xmax = box1.position.x + box1.dimensions.x > box2.position.x + box2.dimensions.x ? box1.position.x + box1.dimensions.x : box2.position.x + box2.dimensions.x;
    ymax = box1.position.y + box1.dimensions.y > box2.position.y + box2.dimensions.y ? box1.position.y + box1.dimensions.y : box2.position.y + box2.dimensions.y;

    return (aabb_2d) {
        .position = {
            .x = clamp(box1.position.x, xmin, xmax),
            .y = clamp(box1.position.y, ymin, ymax)
        },
        .dimensions = {
            .x = clamp(box1.position.x + box1.dimensions.x, xmin, xmax),
            .y = clamp(box1.position.y + box1.dimensions.y, ymin, ymax)
        }
    };
}
aabb_3d aabb_get_intersection_3d(aabb_3d box1, aabb_3d box2) {
    if(!aabb_intersects_3d(box1, box2))
        return (aabb_3d){0};

    float xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = box1.position.x < box2.position.x ? box1.position.x : box2.position.x;
    ymin = box1.position.y < box2.position.y ? box1.position.y : box2.position.y;
    zmin = box1.position.z < box2.position.z ? box1.position.z : box2.position.z;

    xmax = box1.position.x + box1.dimensions.x > box2.position.x + box2.dimensions.x ? box1.position.x + box1.dimensions.x : box2.position.x + box2.dimensions.x;
    ymax = box1.position.y + box1.dimensions.y > box2.position.y + box2.dimensions.y ? box1.position.y + box1.dimensions.y : box2.position.y + box2.dimensions.y;
    zmax = box1.position.z + box1.dimensions.z > box2.position.z + box2.dimensions.z ? box1.position.z + box1.dimensions.z : box2.position.z + box2.dimensions.z;

    return (aabb_3d) {
        .position = {
            .x = clamp(box1.position.x, xmin, xmax),
            .y = clamp(box1.position.y, ymin, ymax),
            .z = clamp(box1.position.z, zmin, zmax)
        },
        .dimensions = {
            .x = clamp(box1.position.x + box1.dimensions.x, xmin, xmax),
            .y = clamp(box1.position.y + box1.dimensions.y, ymin, ymax),
            .z = clamp(box1.position.z + box1.dimensions.z, zmin, zmax)
        }
    };
}

comparison aabb_comparison_2d(aabb_2d box1, aabb_2d box2) {
    return compare(aabb_area_2d(box1), aabb_area_2d(box2));
}
comparison aabb_comparison_3d(aabb_3d box1, aabb_3d box2) {
    return compare(aabb_volume_3d(box1), aabb_volume_3d(box2));
}
