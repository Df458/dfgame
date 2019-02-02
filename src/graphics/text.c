// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "graphics/text.h"

#include "core/check.h"
#include "core/container/array.h"
#include "core/log/log.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include "math/matrix.h"
#include "graphics/font.h"
#include "graphics/mesh.h"
#include "graphics/texture_atlas.h"
#include "graphics/vertex.hd"

/** @brief Represents a piece of text to be rendered
 *
 * @see text.h
 * @see text_new()
 */
typedef struct text {
    font fnt;
    char* str;
    mesh msh;
    alignment_2d align;
    text_wrap wrap;
    float max_width;
    vec2 bounding_size;
}* text;

typedef struct text_line_data {
    aabb_2d box;
    uint16 start;
    uint16 end;
} text_line_data;


glyph* glyph_to_verts(text t, vt_pt* buffer, int index, vec2 offset, float line_height) {
    float atlas_size = font_get_texture(t->fnt).width;
    glyph* gp = font_get_glyph(t->fnt, (uint16)t->str[index]);
    if(gp && gp->advance != 0) {
        aabb_2d box = font_get_glyph_bounds(t->fnt, gp->texture_index);
        // Top-left
        buffer[0].position.xy = vec2_add(offset, (vec2){.x = gp->bearing.x, .y = line_height-gp->bearing.y});
        buffer[0].position.z = 0;
        buffer[0].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
        // Bottom-left
        buffer[1].position.xy = vec2_add(offset, (vec2){.x = gp->bearing.x, .y = line_height + box.dimensions.y - gp->bearing.y});
        buffer[1].position.z = 0;
        buffer[1].uv = vec2_mul((vec2){.x = box.position.x, .y = box.position.y}, 1 / atlas_size);
        // Bottom-right
        buffer[2].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp->bearing.x, .y = line_height + box.dimensions.y - gp->bearing.y});
        buffer[2].position.z = 0;
        buffer[2].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
        // Top-left
        buffer[3].position.xy = vec2_add(offset, (vec2){.x = gp->bearing.x, .y = line_height-gp->bearing.y});
        buffer[3].position.z = 0;
        buffer[3].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
        // Bottom-right
        buffer[4].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp->bearing.x, .y = line_height+box.dimensions.y - gp->bearing.y});
        buffer[4].position.z = 0;
        buffer[4].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
        // Top-right
        buffer[5].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp->bearing.x, .y = line_height-gp->bearing.y});
        buffer[5].position.z = 0;
        buffer[5].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);

        return gp;
    }

    return NULL;
}

void iter_arrange_lines(text_line_data* line, text t) {
    vt_pt* verts = mesh_get_data(t->msh);

    vec3 offset_value = vec3_zero;
    aabb_2d box = {
        .position = vec2_zero,
        .dimensions = { .x = line->box.dimensions.x, .y = t->bounding_size.y }
    };
    offset_value.xy = vec2_mul(aabb_get_origin_2d(box, t->align), -1);

    for(int i = line->start * 6; i < line->end * 6; ++i) {
        verts[i].position = vec_add(verts[i].position, offset_value);
    }

    mesh_update(t->msh);
}

text_line_data update_add_line(text t, array lines, text_line_data old, uint16 cursor, vec2* offset) {
    float height = font_get_height(t->fnt);

    offset->x = 0;
    offset->y += height;

    old.end = cursor;
    array_add(lines, old);
    if(old.box.dimensions.x > t->bounding_size.x) {
        t->bounding_size.x = old.box.dimensions.x;
    }

    text_line_data new_line = (text_line_data){ .box=aabb_2d_zero, .start=0, .end=0 };
    new_line.box.position.y = offset->y;
    new_line.start = cursor + 1;

    t->bounding_size.y += height;

    return new_line;
}

void text_update_mesh(text t) {
    uint16 len = strlen(t->str);
    if(!len)
        return;

    vt_pt* buf = mscalloc(len * 6, vt_pt);
    array line_data = array_mnew_ordered(text_line_data, 4);

    float height = font_get_height(t->fnt);
    t->bounding_size.x = 0;
    t->bounding_size.y = height;

    vec2 offset = vec2_zero;

    text_line_data current_data = (text_line_data){ .box=aabb_2d_zero, .start=0, .end=0 };

    bool line_has_space = false;

    uint16 i = 0;
    for(; i < len; ++i) {
        float width = 0;
        float advance = 0;

        // Calculate width/advance for the new character
        if(t->str[i] == ' ' || t->str[i] == '\t') { // Spaces
            if(eq0(current_data.box.dimensions.x))
                continue;

            line_has_space = true;
            advance = height * (t->str[i] == ' ' ? 0.5f : 2.0f);
        } else if(t->str[i] == '\n') { // Newline
            current_data = update_add_line(t, line_data, current_data, i, &offset);
            line_has_space = false;
            continue;
        } else { // Regular text
            glyph* gp = glyph_to_verts(t, &buf[i * 6], i, offset, height);
            if(gp) {
                width = font_get_glyph_bounds(t->fnt, gp->texture_index).dimensions.x;
                advance = gp->advance;
            }
        }

        // Wrapping
        if(!eq0(t->max_width) && !eq0(current_data.box.dimensions.x) && current_data.box.dimensions.x + width > t->max_width && (line_has_space || t->wrap != TEXT_WRAP_WORD)) {
            current_data = update_add_line(t, line_data, current_data, i, &offset);
            line_has_space = false;

            if(!eq0(width)) {
                if(t->wrap == TEXT_WRAP_CHARACTER) {
                    current_data.start--;

                    // Shift last character
                    glyph_to_verts(t, &buf[i * 6], i, offset, height);
                    offset.x += advance;
                    current_data.box.dimensions.x = offset.x;
                } else {
                    // Find the satrt of the last word so that we can work from there
                    int j = i;
                    for(; j >= 0 && t->str[j] != ' ' && t->str[j] != '\n' && t->str[j] != '\t'; --j);

                    text_line_data* prev_data = array_get(line_data, array_get_length(line_data) - 1);
                    prev_data->end -= i - j;
                    current_data.start -= i - j;

                    // Shift last word
                    for(j++; j <= i; j++) {
                        glyph* gp = glyph_to_verts(t, &buf[j * 6], j, offset, height);
                        if(gp) {
                            offset.x += gp->advance;
                            current_data.box.dimensions.x = offset.x;
                        }
                    }

                    prev_data->box.dimensions.x -= offset.x;
                }
            }
        } else {
            offset.x += advance;
            current_data.box.dimensions.x = offset.x;
        }

        t->bounding_size.x = max(t->bounding_size.x, current_data.box.dimensions.x);
    }
    current_data.end = i;
    if(current_data.start < current_data.end)
        array_add(line_data, current_data);

    mesh_free(t->msh);
    t->msh = mesh_new(len * 6, buf, NULL);
    sfree(buf);

    array_foreach(line_data, i) {
        iter_arrange_lines(i.data, t);
    }
    array_free(line_data);
}

void text_set_str_va(text t, const char* s, va_list args) {
    if(t->str)
        sfree(t->str);

    char* new_str = vsaprintf(s, args);

    if(!new_str)
        return;

    t->str = new_str;

    text_update_mesh(t);
}

// ---------------------------------------------------------------------------------

text text_new(font f, const char* s, ...) {
    text t = mscalloc(1, struct text);
    t->fnt = f;
    t->msh = NULL;
    t->str = NULL;
    t->align = ALIGN_DEFAULT;
    t->wrap = TEXT_WRAP_DEFAULT;
    t->max_width = 0;
    t->bounding_size = vec2_zero;

    va_list args;
    va_start(args, s);
    text_set_str_va(t, s, args);
	va_end(args);

    return t;
}

void _text_free(text t, bool free_src) {
    check_return(t, "Text is NULL", );

    sfree(t->str)

    if(t->msh)
        mesh_free(t->msh);

    if(free_src)
        font_free(t->fnt);

    sfree(t);
}

void text_set_str(text t, const char* s, ...) {
    check_return(t, "Text is NULL", );

    va_list args;
    va_start(args, s);
    text_set_str_va(t, s, args);
	va_end(args);
}

char* text_get_str(const text t) {
    check_return(t, "Text is NULL", NULL);

    return nstrdup(t->str);
}

mesh text_get_mesh(const text t) {
    check_return(t, "Text is NULL", NULL);

    return t->msh;
}

void text_set_align(text t, alignment_2d align) {
    check_return(t, "Text is NULL", );
    check_return(align <= ALIGN_LAST, "Invalid text alignment 0x%x", , align);

    t->align = align;
    text_update_mesh(t);
}
alignment_2d text_get_align(const text t) {
    check_return(t, "Text is NULL", ALIGN_DEFAULT);

    return t->align;
}

void text_set_wrap(text t, text_wrap wrap) {
    check_return(t, "Text is NULL", );
    check_return(wrap <= TEXT_WRAP_LAST, "Invalid text wrap 0x%x", , wrap);

    t->wrap = wrap;
    text_update_mesh(t);
}
text_wrap text_get_wrap(const text t) {
    check_return(t, "Text is NULL", TEXT_WRAP_DEFAULT);

    return t->wrap;
}

vec2 text_get_bounds(const text t) {
    check_return(t, "Text is NULL", vec2_zero);

    return t->bounding_size;
}

void text_set_max_width(text t, float width) {
    check_return(t, "Text is NULL", );

    t->max_width = width;
    text_update_mesh(t);
}
float text_get_max_width(const text t) {
    check_return(t, "Text is NULL", 0);

    return t->max_width;
}

font text_get_font(const text t) {
    check_return(t, "Text is NULL", NULL);

    return t->fnt;
}
void text_set_font(text t, font f) {
    check_return(t, "Text is NULL", );
    check_return(f, "Font is NULL", );

    t->fnt = f;
    text_update_mesh(t);
}

void text_draw(const text t, shader s, mat4 m) {
    check_return(t, "Text is NULL", );
    check_return(t->fnt, "Text has no font", );

    if(t->msh) {
        glUseProgram(s.id);

        shader_bind_uniform_name(s, "u_transform", m);
        shader_bind_uniform_texture_name(s, "u_texture", font_get_texture(t->fnt), GL_TEXTURE0);
        vec2 v0 = (vec2){.x=0,.y=0};
        vec2 v1 = (vec2){.x=1,.y=1};
        shader_bind_uniform_name(s, "uv_offset", v0);
        shader_bind_uniform_name(s, "uv_scale", v1);

        mesh_render(s, t->msh, GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    }
}
