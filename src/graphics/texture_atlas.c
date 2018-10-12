// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "texture_atlas.h"

#include "check.h"
#include "container/array.h"
#include "memory/alloc.h"
#include "power.h"

typedef struct atlas_box {
    aabb_2d box;
    vec3 free_marker;
} atlas_box;

bool find_fitting_atlas_box(void* o1, void* o2, void* user) {
    atlas_box a = *(atlas_box*)o1; // Container
    atlas_box* a_p = (atlas_box*)o1;
    aabb_2d b = *(aabb_2d*)o2; // New box

    if(a.box.dimensions.x < b.dimensions.x || a.box.dimensions.y - a.free_marker.y < b.dimensions.y)
        return false;

    if(a.box.dimensions.x - a.free_marker.x < b.dimensions.x) {
        if(a.box.dimensions.y - a.free_marker.z < b.dimensions.y)
            return false;

        a_p->free_marker.x = 0;
        a_p->free_marker.y = a.free_marker.z;
        a_p->free_marker.z += b.dimensions.y;
    } else if(a.free_marker.z - a.free_marker.y < b.dimensions.y) {
        a_p->free_marker.z = a.free_marker.y + b.dimensions.y;
    }

    return true;
}

typedef struct texture_atlas {
    gltex texture_data;
    array textures;
    array free_space;
}* texture_atlas;

texture_atlas texture_atlas_new() {
    texture_atlas atlas = salloc(sizeof(struct texture_atlas));
    atlas->texture_data = gltex_new(GL_TEXTURE_2D, 256, 256);
    uint8 clearcolor[4] = { 0, 0, 0, 0 };
    glClearTexImage(atlas->texture_data.handle, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearcolor);

    atlas->textures = array_mnew(aabb_2d, 8);
    atlas->free_space = array_mnew(atlas_box, 8);

    atlas_box initial = { .box=aabb_2d_zero, .free_marker=vec3_zero };
    initial.box = (aabb_2d) {
        .position = vec2_zero,
        .dimensions = (vec2){ .x = 256, .y = 256 }
    };
    array_add(atlas->free_space, initial);

    return atlas;
}

aabb_2d texture_atlas_insert_box(texture_atlas atlas, aabb_2d box) {
    uint16 new_size = atlas->texture_data.width;
    atlas_box* found_box = NULL;

    do {
        int32 res = array_findp(atlas->free_space, &box, find_fitting_atlas_box, NULL);

        if(res == -1) {
            uint16 prev_size = new_size;
            new_size = next_power_of_two(new_size);
            uint16 len = array_get_length(atlas->free_space);

            for(uint16 i = 0; i < len; ++i) {
                atlas_box* box = array_get(atlas->free_space, i);

                if(box->box.position.x + box->box.dimensions.x >= prev_size) {
                    if(box->free_marker.y == 0) {
                        box->box.dimensions.x += new_size - prev_size;
                    } else {
                        atlas_box new_box = {
                            .box = (aabb_2d) {
                                .position = (vec2){ .x = prev_size, .y = box->box.position.y },
                                .dimensions = (vec2){ .x = new_size - prev_size, .y = box->box.dimensions.y }
                            }, .free_marker=vec3_zero
                        };
                        array_add(atlas->free_space, new_box);
                    }
                }
            }
            atlas_box new_box = {
                .box = (aabb_2d) {
                    .position = (vec2){ .x = 0, .y = prev_size },
                    .dimensions = (vec2){ .x = new_size, .y = new_size - prev_size }
                },
                .free_marker=vec3_zero
            };
            array_add(atlas->free_space, new_box);
        } else {
            found_box = array_get(atlas->free_space, res);
        }
    } while(!found_box && new_size <= 4096);

    check_return(found_box, "Can't pack a texture of "vec2_printstr" in atlas (Current size [%d, %d]).", aabb_2d_zero, vec2_decomp(box.dimensions), atlas->texture_data.width, atlas->texture_data.height);

    if(new_size != atlas->texture_data.width)
    {
        gltex new_tex = gltex_new(GL_TEXTURE_2D, new_size, new_size);
        uint8 clearcolor[4] = { 0, 0, 0, 0 };
        glClearTexImage(new_tex.handle, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearcolor);

        glCopyImageSubData(atlas->texture_data.handle, atlas->texture_data.type, 0, 0, 0, 0, new_tex.handle, new_tex.type, 0, 0, 0, 0, atlas->texture_data.width, atlas->texture_data.height, 1);
        glDeleteTextures(1, &atlas->texture_data.handle);
        atlas->texture_data = new_tex;
    }

    box.position = vec2_add(found_box->box.position, found_box->free_marker.xy);
    found_box->free_marker.x += box.dimensions.x;
    array_add(atlas->textures, box);

    return box;
}

int16 texture_atlas_add_gl(texture_atlas atlas, gltex tex, GLenum mode) {
    aabb_2d box = (aabb_2d){
        .dimensions = {
            .x=tex.width,
            .y=tex.height,
        }
    };
    aabb_2d container = texture_atlas_insert_box(atlas, box);

    if(!container.dimensions.x)
        return -1;

    glCopyImageSubData(tex.handle, tex.type, 0, 0, 0, 0, atlas->texture_data.handle, atlas->texture_data.type, 0, container.position.x, container.position.y, 0, container.dimensions.x, container.dimensions.y, 1);

    return array_get_length(atlas->textures) - 1;
}

int16 texture_atlas_add_raw(texture_atlas atlas, rawtex tex, GLenum mode) {
    aabb_2d box = (aabb_2d){
        .dimensions = {
            .x=tex.width,
            .y=tex.height,
        }
    };
    aabb_2d container = texture_atlas_insert_box(atlas, box);

    if(container.dimensions.x == 0)
        return -1;

    glBindTexture(GL_TEXTURE_2D, atlas->texture_data.handle);
    for(int i = 0; i < tex.height; ++i)
        glTexSubImage2D(GL_TEXTURE_2D, 0, container.position.x, container.position.y + i, container.dimensions.x, 1, mode, GL_UNSIGNED_BYTE, tex.data + ((tex.height - i - 1) * tex.width * tex.elements));

    return array_get_length(atlas->textures) - 1;
}

aabb_2d texture_atlas_get(texture_atlas atlas, int16 index) {
    check_return(array_get_length(atlas->textures) > index, "Can't get texture bounds in atlas: Index %d is out of bounds", aabb_2d_zero, index);

    return *(aabb_2d*)array_get(atlas->textures, index);
}

void texture_atlas_remove(texture_atlas atlas, int16 index) {
    array_remove_at(atlas->textures, index);
}

// Frees the given atlas, and all resources contained within it
// NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _texture_atlas_free(texture_atlas atlas) {
    check_return(atlas, "Can't destroy atlas: Atlas is null", );

    array_free(atlas->textures);
    array_free(atlas->free_space);
    gltex_cleanup(&atlas->texture_data);
    sfree(atlas);
}

gltex texture_atlas_get_texture(texture_atlas atlas) {
    return atlas->texture_data;
}
