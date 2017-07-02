// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "texture_atlas.h"

#include "check.h"
#include "container/array.h"
#include "memory/alloc.h"

bool find_fitting_atlas_box(void* o1, void* o2, void* user) {
    aabb_2d box = *(aabb_2d*)o2;
    aabb_2d container = *(aabb_2d*)o1;

    return box.dimensions.x <= container.dimensions.x && box.dimensions.y <= container.dimensions.y;
}

typedef struct texture_atlas {
    gltex texture_data;
    uarray textures;
    vec3 free_space;
}* texture_atlas;

// TODO: Resize if the atlas fills
texture_atlas texture_atlas_new() {
    texture_atlas atlas = salloc(sizeof(struct texture_atlas));
    atlas->texture_data = (gltex) {
        .width = 4096,
        .height = 4096,
        .type = GL_TEXTURE_2D,
    };
    glGenTextures(1, &atlas->texture_data.handle);

    glBindTexture(GL_TEXTURE_2D, atlas->texture_data.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    atlas->textures = uarray_new(8);
    atlas->free_space = (vec3){0};

    return atlas;
}

aabb_2d texture_atlas_insert_box(texture_atlas atlas, aabb_2d box) {
    check_return(box.width <= atlas->texture_data.width && box.height <= (atlas->texture_data.height - atlas->free_space.y) && (box.width <= (atlas->texture_data.width - atlas->free_space.x) || box.height <= (atlas->texture_data.height - atlas->free_space.z)), "Can't pack texture of "vec2_printstr" in atlas (free "vec3_printstr" [%d %d])", (aabb_2d){0}, vec2_decomp(box.dimensions), vec3_decomp(atlas->free_space), atlas->texture_data.width, atlas->texture_data.height);

    if(box.width < atlas->texture_data.width - atlas->free_space.x) {
        box.position = atlas->free_space.xy;
        atlas->free_space.x += box.width;
        atlas->free_space.z = max(atlas->free_space.z, atlas->free_space.y + box.height);
    } else {
        box.x = 0;
        box.y = atlas->free_space.z;
        atlas->free_space.x = box.width;
        atlas->free_space.y = atlas->free_space.z;
        atlas->free_space.z = max(atlas->free_space.z, atlas->free_space.y + box.height);
    }

    array_copyadd(atlas->textures, &box, sizeof(aabb_2d));

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

    glCopyImageSubData(tex.handle, tex.type, 0, 0, 0, 0, atlas->texture_data.handle, atlas->texture_data.type, 0, container.position.x, container.position.y, 0, container.dimensions.x, container.dimensions.y, 0);

    return array_size(atlas->textures) - 1;
}

int16 texture_atlas_add_raw(texture_atlas atlas, rawtex tex, GLenum mode) {
    aabb_2d box = (aabb_2d){
        .dimensions = {
            .x=tex.width,
            .y=tex.height,
        }
    };
    aabb_2d container = texture_atlas_insert_box(atlas, box);

    if(!container.dimensions.x)
        return -1;

    glBindTexture(GL_TEXTURE_2D, atlas->texture_data.handle);
    for(int i = 0; i < tex.height; ++i)
        glTexSubImage2D(GL_TEXTURE_2D, 0, container.position.x, container.position.y + i, container.dimensions.x, 1, mode, GL_UNSIGNED_BYTE, tex.data + ((tex.height - i - 1) * tex.width * tex.elements));

    return array_size(atlas->textures) - 1;
}

aabb_2d texture_atlas_get(texture_atlas atlas, int16 index) {
    check_return(array_size(atlas->textures) > index, "Can't get texture bounds in atlas: Index %d is out of bounds", (aabb_2d){0}, index);

    return *(aabb_2d*)array_get(atlas->textures, index);
}

void texture_atlas_free(texture_atlas atlas) {
    check_return(atlas, "Can't destroy atlas: Atlas is null", );

    array_free_deep(atlas->textures);
    glDeleteTextures(1, &atlas->texture_data.handle);
    sfree(atlas);
}

gltex texture_atlas_get_texture(texture_atlas atlas) {
    return atlas->texture_data;
}
