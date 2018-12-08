#include "alignment.h"

#include "core/check.h"

// Get a point aligned with the given aabb
vec2 aabb_get_origin_2d(aabb_2d box, alignment_2d align) {
    check_return(align >= 0 && align <= ALIGN_LAST, "Invalid alignment 0x%x provided", vec2_zero, align);

    vec2 result = box.position;

    // X alignment
    switch(align % 3) {
        case 1:
            result.x += box.dimensions.x * 0.5f;
        break;
        case 2:
            result.x += box.dimensions.x;
        break;
    }
    // Y alignment
    switch(align / 3) {
        case 1:
            result.y += box.dimensions.y * 0.5f;
        break;
        case 2:
            result.y += box.dimensions.y;
        break;
    }

    return result;
}
vec3 aabb_get_origin_3d(aabb_3d box, alignment_3d align) {
    check_return(align >= 0 && align <= ALIGN_3D_LAST, "Invalid alignment 0x%x provided", vec3_zero, align);

    vec3 result = box.position;

    // X alignment
    switch(align % 3) {
        case 1:
            result.x += box.dimensions.x * 0.5f;
        break;
        case 2:
            result.x += box.dimensions.x;
        break;
    }
    // Y alignment
    switch(align / 9) {
        case 1:
            result.y += box.dimensions.y * 0.5f;
        break;
        case 2:
            result.y += box.dimensions.y;
        break;
    }
    // Z alignment
    switch((align / 3) % 3) {
        case 1:
            result.z += box.dimensions.z * 0.5f;
        break;
        case 2:
            result.z += box.dimensions.z;
        break;
    }

    return result;
}

// Get an aabb with the dimensions of box2 aligned to a point
aabb_2d aabb_align_point_2d(aabb_2d box, vec2 point, alignment_2d align) {
    check_return(align >= 0 && align <= ALIGN_LAST, "Invalid alignment 0x%x provided", aabb_2d_zero, align);

    aabb_2d result = box;
    result.position = point;

    // X alignment
    switch(align % 3) {
        case 1:
            result.position.x -= box.dimensions.x * 0.5f;
        break;
        case 2:
            result.position.x -= box.dimensions.x;
        break;
    }
    // Y alignment
    switch(align / 3) {
        case 1:
            result.position.y -= box.dimensions.y * 0.5f;
        break;
        case 2:
            result.position.y -= box.dimensions.y;
        break;
    }

    return result;
}
aabb_3d aabb_align_point_3d(aabb_3d box, vec3 point, alignment_3d align) {
    check_return(align >= 0 && align <= ALIGN_3D_LAST, "Invalid alignment 0x%x provided", aabb_3d_zero, align);

    aabb_3d result = box;
    result.position = point;

    // X alignment
    switch(align % 3) {
        case 1:
            result.position.x -= box.dimensions.x * 0.5f;
        break;
        case 2:
            result.position.x -= box.dimensions.x;
        break;
    }
    // Y alignment
    switch(align / 9) {
        case 1:
            result.position.y -= box.dimensions.y * 0.5f;
        break;
        case 2:
            result.position.y -= box.dimensions.y;
        break;
    }
    // Z alignment
    switch((align / 3) % 3) {
        case 1:
            result.position.z -= box.dimensions.z * 0.5f;
        break;
        case 2:
            result.position.z -= box.dimensions.z;
        break;
    }

    return result;
}

// Get an aabb with the dimensions of box aligned to box2
aabb_2d aabb_align_box_2d(aabb_2d box, aabb_2d box2, alignment_2d align) {
    check_return(align >= 0 && align <= ALIGN_LAST, "Invalid alignment 0x%x provided", aabb_2d_zero, align);
    check_return(box.dimensions.x <= box2.dimensions.x && box.dimensions.y <= box2.dimensions.y, "Box "aabb_2d_printstr" can't be aligned to container "aabb_2d_printstr" because it's larger", aabb_2d_zero, aabb_2d_decomp(box), aabb_2d_decomp(box2));

    return aabb_align_point_2d(box, aabb_get_origin_2d(box2, align), align);
}
aabb_3d aabb_align_box_3d(aabb_3d box, aabb_3d box2, alignment_3d align) {
    check_return(align >= 0 && align <= ALIGN_3D_LAST, "Invalid alignment 0x%x provided", aabb_3d_zero, align);
    check_return(
               box.dimensions.x <= box2.dimensions.x
            && box.dimensions.y <= box2.dimensions.y
            && box.dimensions.z <= box2.dimensions.z,
            "Box "aabb_3d_printstr" can't be aligned to container "aabb_3d_printstr" because it's larger",
            aabb_3d_zero, aabb_3d_decomp(box), aabb_3d_decomp(box2));

    return aabb_align_point_3d(box, aabb_get_origin_3d(box2, align), align);
}
