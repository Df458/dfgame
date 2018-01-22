// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "text.h"

#include "container/array.h"
#include "font.h"
#include "log/log.h"
#include "memory/alloc.h"
#include "matrix.h"
#include "mesh.h"
#include "texture_atlas.h"
#include "vertex.hd"
#include <string.h>

void glyph_to_verts(vt_pt* buffer, glyph gp, aabb_2d box, vec2 offset, float atlas_size, float line_height) {
    // Top-left
    buffer[0].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = line_height-gp.bearing.y});
    buffer[0].position.z = 0;
    buffer[0].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
    // Bottom-left
    buffer[1].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = line_height + box.dimensions.y - gp.bearing.y});
    buffer[1].position.z = 0;
    buffer[1].uv = vec2_mul((vec2){.x = box.position.x, .y = box.position.y}, 1 / atlas_size);
    // Bottom-right
    buffer[2].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = line_height + box.dimensions.y - gp.bearing.y});
    buffer[2].position.z = 0;
    buffer[2].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
    // Top-left
    buffer[3].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = line_height-gp.bearing.y});
    buffer[3].position.z = 0;
    buffer[3].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
    // Bottom-right
    buffer[4].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = line_height+box.dimensions.y - gp.bearing.y});
    buffer[4].position.z = 0;
    buffer[4].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
    // Top-right
    buffer[5].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = line_height-gp.bearing.y});
    buffer[5].position.z = 0;
    buffer[5].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
}

typedef struct text {
    font fnt;
    char* str;
    mesh msh;
    text_alignment align;
    vec2 bounding_size;
}* text;

typedef struct text_line_data {
    aabb_2d box;
    uint16 start;
    uint16 end;
} text_line_data;


iter_result iter_arrange_lines(void* data, void* user) {
    text t = user;
    text_line_data* line = data;
    vt_pt* verts = mesh_get_data(t->msh);

    vec3 offset_value = {0};

    switch(t->align) {
        case TEXT_ALIGN_TOP:
            offset_value.x = line->box.dimensions.x * -0.5f;
            break;
        case TEXT_ALIGN_BOTTOM:
            offset_value.x = line->box.dimensions.x * -0.5f;
            offset_value.y = t->bounding_size.y * -1.0f;
            break;
        case TEXT_ALIGN_LEFT:
            offset_value.y = t->bounding_size.y * -0.5f;
            break;
        case TEXT_ALIGN_RIGHT:
            offset_value.x = line->box.dimensions.x * -1.0f;
            offset_value.y = t->bounding_size.y * -0.5f;
            break;

        case TEXT_ALIGN_TOP_LEFT:
            break;
        case TEXT_ALIGN_TOP_RIGHT:
            offset_value.x = line->box.dimensions.x * -1.0f;
            break;
        case TEXT_ALIGN_BOTTOM_LEFT:
            offset_value.y = t->bounding_size.y * -1.0f;
            break;
        case TEXT_ALIGN_BOTTOM_RIGHT:
            offset_value.x = line->box.dimensions.x * -1.0f;
            offset_value.y = t->bounding_size.y * -1.0f;
            break;

        case TEXT_ALIGN_CENTER:
            offset_value.x = line->box.dimensions.x * -0.5f;
            offset_value.y = t->bounding_size.y * -0.5f;
            break;
    }

    for(int i = line->start * 6; i < line->end * 6; ++i)
        verts[i].position = vec_add(verts[i].position, offset_value);

    mesh_update(t->msh);

    return iter_continue;
}

void text_update_mesh(text t) {
    uint16 len = strlen(t->str);

    vt_pt* buf = scalloc(len * 6, sizeof(vt_pt));

    sarray line_data = sarray_new(4);

    vec2 offset = (vec2){0};
    float height = font_get_height(t->fnt);
    text_line_data current_data = {0};
    t->bounding_size.x = 0;
    t->bounding_size.y = height;
    int i = 0;
    for(i = 0; i < len; ++i) {
        if(t->str[i] == ' ') {
            offset.x += height * 0.5;
            current_data.box.dimensions.x = offset.x;
            continue;
        } else if(t->str[i] == '\n') {
            offset.x = 0;
            offset.y += height;

            current_data.end = i;
            array_copyadd_simple(line_data, current_data);
            if(current_data.box.dimensions.x > t->bounding_size.x)
                t->bounding_size.x = current_data.box.dimensions.x;
            current_data = (text_line_data){0};
            current_data.box.position.y = offset.y;
            current_data.start = i+ 1;

            t->bounding_size.y += height;
            continue;
        } else if(t->str[i] == '\t') {
            offset.x += height * 2;
            current_data.box.dimensions.x = offset.x;
            continue;
        }

        glyph* gp = font_get_glyph(t->fnt, (int16)t->str[i]);
        if(!gp)
            continue;

        current_data.box.dimensions.x = offset.x;
        if(gp->advance != 0) {
            aabb_2d box = font_get_glyph_bounds(t->fnt, gp->texture_index);
            glyph_to_verts(&buf[i * 6], *gp, box, offset, font_get_texture(t->fnt).width, height);
            current_data.box.dimensions.x += box.dimensions.x;
            offset.x += gp->advance;
        } else {
            offset.x += height * 0.5;
        }
    }
    current_data.end = i;
    if(current_data.start < current_data.end)
        array_copyadd_simple(line_data, current_data);

    mesh_free(t->msh);
    t->msh = mesh_new(len * 6, buf);

    array_foreach(line_data, iter_arrange_lines, t);

    array_free_deep(line_data);
}

void text_set_str_va(text t, const char* s, va_list args) {
    if(t->str)
        sfree(t->str);

    if(!s)
        return;

    va_list tempargs;
    size_t length;
	va_copy(tempargs, args);

    length = vsnprintf(0, 0, s, tempargs);
    va_end(tempargs);

	++length;
    t->str = mscalloc(length, char);

    vsnprintf(t->str, length, s, args);

    text_update_mesh(t);
}


text text_new(font f, const char* s, ...) {
    text t = salloc(sizeof(struct text));
    t->fnt = f;
    t->msh = NULL;
    t->str = NULL;
    t->align = TEXT_ALIGN_DEFAULT;

    va_list args;
    va_start(args, s);
    text_set_str_va(t, s, args);
	va_end(args);

    return t;
}

void text_set_str(text t, const char* s, ...) {
    va_list args;
    va_start(args, s);
    text_set_str_va(t, s, args);
	va_end(args);
}

mesh text_get_mesh(text t) {
    return t->msh;
}

void text_set_align(text t, text_alignment align) {
    t->align = align;
    text_update_mesh(t);
}

text_alignment text_get_align(text t) {
    return t->align;
}

font text_get_font(text t) {
    return t->fnt;
}

void text_set_font(text t, font f) {
    t->fnt = f;
    text_update_mesh(t);
}

void text_draw(text t, shader s, mat4 m) {
    glUseProgram(s.id);
    shader_bind_uniform_name(s, "u_transform", m);
    shader_bind_uniform_texture_name(s, "u_texture", font_get_texture(t->fnt), GL_TEXTURE0);
    vec2 v0 = (vec2){.x=0,.y=0};
    vec2 v1 = (vec2){.x=1,.y=1};
    shader_bind_uniform_name(s, "uv_offset", v0);
    shader_bind_uniform_name(s, "uv_scale", v1);
    mesh_render(s, t->msh, GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
}

void _text_free(text t, bool free_src) {
    sfree(t->str)
    mesh_free(t->msh);

    if(free_src)
        font_free(t->fnt);
    sfree(t);
}
