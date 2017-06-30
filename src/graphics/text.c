// Log category, used to filter logs
#define LOG_CATEGORY "Graphics"

#include "text.h"

#include "font.h"
#include "log/log.h"
#include "memory/alloc.h"
#include "mesh.h"
#include "texture_atlas.h"
#include "vertex.hd"
#include <string.h>

void glyph_to_verts(vt_pt* buffer, glyph gp, aabb_2d box, vec2 offset, float atlas_size) {
    // Top-left
    buffer[0].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = -gp.bearing.y});
    buffer[0].position.z = 0;
    buffer[0].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
    // Bottom-left
    buffer[1].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = box.dimensions.y - gp.bearing.y});
    buffer[1].position.z = 0;
    buffer[1].uv = vec2_mul((vec2){.x = box.position.x, .y = box.position.y}, 1 / atlas_size);
    // Bottom-right
    buffer[2].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = box.dimensions.y - gp.bearing.y});
    buffer[2].position.z = 0;
    buffer[2].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
    // Top-left
    buffer[3].position.xy = vec2_add(offset, (vec2){.x = gp.bearing.x, .y = -gp.bearing.y});
    buffer[3].position.z = 0;
    buffer[3].uv = vec2_mul((vec2){.x = box.position.x, .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
    // Bottom-right
    buffer[4].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = box.dimensions.y - gp.bearing.y});
    buffer[4].position.z = 0;
    buffer[4].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = box.position.y}, 1 / atlas_size);
    // Top-right
    buffer[5].position.xy = vec2_add(offset, (vec2){.x = box.dimensions.x + gp.bearing.x, .y = -gp.bearing.y});
    buffer[5].position.z = 0;
    buffer[5].uv = vec2_mul((vec2){.x = (box.position.x + box.dimensions.x), .y = (box.position.y + box.dimensions.y)}, 1 / atlas_size);
}

typedef struct text {
    font fnt;
    char* str;
    /* glyph* buffer; */
    mesh msh;
}* text;

text text_new(font f, const char* s) {
    text t = salloc(sizeof(struct text));
    t->fnt = f;
    t->msh = NULL;
    t->str = NULL;
    text_set_str(t, s);
    return t;
}

void text_set_str(text t, const char* s) {
    if(t->str)
        sfree(t->str);
    t->str = strdup(s);

    uint16 len = strlen(s);

    vt_pt* buf = scalloc(len * 6, sizeof(vt_pt));

    vec2 offset = (vec2){0};
    float height = font_get_height(t->fnt);
    for(int i = 0; i < len; ++i) {
        if(s[i] == ' ') {
            offset.x += height * 0.5;
            continue;
        } else if(s[i] == '\n') {
            offset.x = 0;
            offset.y += height;
            continue;
        } else if(s[i] == '\t') {
            offset.x += height * 2;
            continue;
        }

        glyph gp = font_get_glyph(t->fnt, (int16)s[i]);
        gp.advance = gp.advance;

        // TODO: Check glyph error

        if(gp.advance != 0) {
            glyph_to_verts(&buf[i * 6], gp, font_get_glyph_bounds(t->fnt, gp.texture_index), offset, font_get_texture(t->fnt).width);
            offset.x += gp.advance;
        } else {
            offset.x += height * 0.5;
        }
    }
    mesh_free(t->msh);
    t->msh = mesh_new(len * 6, buf);
}

mesh text_get_mesh(text t) {
    return t->msh;
}
