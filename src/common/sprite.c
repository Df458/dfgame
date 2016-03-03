#include <stdlib.h>
#include <png.h>
#include <string.h>

#include "sprite_def.h"
#include "sprite.h"
#include "render.h"
#include "util.h"
#include "xml_util.h"

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

uint8_t box_contains(struct texture_atlas_box holder, struct texture_atlas_box child)
{
    if(holder.size_x >= child.size_x && holder.size_y >= child.size_y)
        return 1;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////

animation* create_animation()
{
    animation* a = malloc(sizeof(animation));

    a->orients      = 0;
    a->dimensions_x = 0;
    a->dimensions_y = 0;
    a->origin_x     = 0;
    a->origin_y     = 0;
    a->length       = 0;
    a->delay        = 0;
    a->autoplay     = false;
    a->autoloop     = false;
    a->handle       = 0;

    return a;
}

void destroy_animation(animation* anim)
{
    if(anim->handle)
        free(anim->handle);

    return;
}

bool save_animation_to_resource(animation* anim, resource_pair)
{
    stub(false);
}

animation* load_resource_to_animation(resource_pair)
{
    stub(0);
}


spriteset* create_spriteset()
{
	spriteset* spriteset_data = malloc(sizeof(spriteset));
    spriteset_data->atlas = 0;
    spriteset_data->animations = 0;
    spriteset_data->animation_count = 0;

    return spriteset_data;
}

void destroy_spriteset(spriteset* spr)
{
    if(spr->atlas) {
            destroy_texture(spr->atlas);
        for(uint8_t i = 0; i < spr->animation_count; ++i)
            free(spr->animations[i].handle);
        free(spr->animations);
    }
    free(spr);
}

// TODO: Add support for paths in spriteset files
// TODO: Rework this to work better with the new setup
spriteset* load_resource_to_spriteset(resource_pair)
{
    xmlDocPtr doc = load_resource_to_xml(resource_location, resource_name);
    if(!doc)
        return 0;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    for(; root; root = root->next)
        if(root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"spriteset"))
            break;
    if(!root)
        return 0;
    spriteset* spr = malloc(sizeof(spriteset));
    spr->animations = malloc(sizeof(struct animation));
    spr->atlas = malloc(sizeof(texture));
    spr->animation_count = 0;
    uint8_t** buffers = malloc(sizeof(uint8_t*));
    /*struct texture_atlas_box* boxes = malloc(sizeof(struct texture_atlas_box));*/
    for(xmlNodePtr node = root->children; node; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation")) {
            spr->animation_count++;
            if(spr->animation_count > 1) {
                spr->animations = realloc(spr->animations, spr->animation_count * sizeof(struct animation));
                buffers = realloc(buffers, spr->animation_count * sizeof(uint8_t*));
                /*boxes = realloc(boxes, spr->animation_count * sizeof(struct texture_atlas_box));*/
            }
            spr->animations[spr->animation_count - 1].handle = "idle";
            spr->animations[spr->animation_count - 1].orients = 0;
            spr->animations[spr->animation_count - 1].length = 1;
            spr->animations[spr->animation_count - 1].delay = 1;
            spr->animations[spr->animation_count - 1].autoloop = false;
            spr->animations[spr->animation_count - 1].autoplay = true;
            spr->animations[spr->animation_count - 1].origin_x = 0;
            spr->animations[spr->animation_count - 1].origin_y = 0;
            spr->animations[spr->animation_count - 1].speed_mod = 1;
            xmlChar* a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"name"))) {
                spr->animations[spr->animation_count - 1].handle = strdup((char*)a);
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"length"))) {
                spr->animations[spr->animation_count - 1].length = atoi((char*)a);
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"speed"))) {
                spr->animations[spr->animation_count - 1].delay = atoi((char*)a);
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"orients"))) {
                spr->animations[spr->animation_count - 1].orients = atoi((char*)a);
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"loop"))) {
                spr->animations[spr->animation_count - 1].autoloop = strcmp((char*)a, "false") ? true : false;
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"play"))) {
                spr->animations[spr->animation_count - 1].autoplay = strcmp((char*)a, "false") ? true : false;
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"origin_x"))) {
                spr->animations[spr->animation_count - 1].origin_x = atoi((char*)a);
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"origin_y"))) {
                spr->animations[spr->animation_count - 1].origin_y = atoi((char*)a);
                free(a);
            }
            a = 0;
            uint8_t keep = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"file"))) {
                uint16_t w, h;
                buffers[spr->animation_count - 1] = load_resource_to_texture_buffer(resource_location, (char*)a, &w, &h);
                spr->animations[spr->animation_count - 1].box.size_x = w;
                spr->animations[spr->animation_count - 1].box.size_y = h;
                spr->animations[spr->animation_count - 1].box.pos_x = 0;
                spr->animations[spr->animation_count - 1].box.pos_y = 0;
                if(buffers[spr->animation_count - 1])
                    keep = 1;
                free(a);
            }
            if(!keep) {
                spr->animation_count--;
                if(spr->animation_count > 0) {
                    spr->animations = realloc(spr->animations, spr->animation_count * sizeof(struct animation));
                    buffers = realloc(buffers, spr->animation_count * sizeof(uint8_t*));
                }
            }
        }
    }

    if(spr->animation_count == 0) {
        error("Cannot load spriteset: No animations found.\n");
        free(spr->animations);
        free(spr);
        xmlFreeDoc(doc);
        return 0;
    }
    // TODO: Sort by width
    // spr up the initial box for the texture
    struct texture_atlas_box* available = malloc(sizeof(struct texture_atlas_box));
    uint16_t avail_length = 1;
    available[0].pos_x = 0;
    available[0].pos_y = 0;
    available[0].size_x = 1024;
    available[0].size_y = 1024;
    uint16_t width = 1024;
    uint16_t height = 1024;
    uint16_t f_width = 0;
    uint16_t f_height = 0;

    for(int i = 0; i < spr->animation_count; ++i) {
        int16_t selected = -1;
        for(int j = 0; j < avail_length; ++j) {
            if(box_contains(available[j], spr->animations[i].box) && (selected < 0 || available[j].size_x * available[j].size_y > available[selected].size_x * available[selected].size_y)) {
                selected = j;
            }
        }
        if(selected == -1) {
            width += 128;
            height += 128;
            avail_length += 2;
            available = realloc(available, avail_length * sizeof(struct texture_atlas_box));
            available[avail_length - 2].pos_x = width - 128;
            available[avail_length - 2].pos_y = 0;
            available[avail_length - 2].size_x = 128;
            available[avail_length - 2].size_y = height;
            available[avail_length - 1].pos_x = 0;
            available[avail_length - 1].pos_y = height - 128;
            available[avail_length - 2].size_x = width - 128;
            available[avail_length - 2].size_y = 128;
        }
        spr->animations[i].box.pos_x = available[selected].pos_x;
        spr->animations[i].box.pos_y = available[selected].pos_y;
        if(spr->animations[i].box.pos_x + spr->animations[i].box.size_x > f_width)
            f_width = spr->animations[i].box.pos_x + spr->animations[i].box.size_x;
        if(spr->animations[i].box.pos_y + spr->animations[i].box.size_y > f_height)
            f_height = spr->animations[i].box.pos_y + spr->animations[i].box.size_y;
        avail_length--;
        if(spr->animations[i].box.size_x < available[selected].size_x) {
            avail_length++;
            available[selected].pos_x += spr->animations[i].box.size_x;
            available[selected].size_x -= spr->animations[i].box.size_x;
            if(spr->animations[i].box.size_y < available[selected].size_y) {
                avail_length++;
                available = realloc(available, avail_length * sizeof(struct texture_atlas_box));
                available[avail_length - 1].pos_x = spr->animations[i].box.pos_x;
                available[avail_length - 1].pos_y = spr->animations[i].box.pos_y + spr->animations[i].box.size_y;
                available[avail_length - 1].size_x = spr->animations[i].box.size_x;
                available[avail_length - 1].size_y = available[selected].size_y - spr->animations[i].box.size_y;
            }
        } else if(spr->animations[i].box.size_y < available[selected].size_y) {
            avail_length++;
            available[selected].pos_y += spr->animations[i].box.size_y;
            available[selected].size_y -= spr->animations[i].box.size_y;
        } else {
            for(int k = selected + 1; k <= avail_length; ++k) {
                available[k - 1] = available[k];
            }
            available = realloc(available, avail_length * sizeof(struct texture_atlas_box));
        }
    }

    glGenTextures(1, &spr->atlas->handle);
    glBindTexture(GL_TEXTURE_2D, spr->atlas->handle);
    checkGLError();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, f_width, f_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    checkGLError();
    for(int i = 0; i < spr->animation_count; ++i) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, spr->animations[i].box.pos_x, spr->animations[i].box.pos_y, spr->animations[i].box.size_x, spr->animations[i].box.size_y, GL_RGBA, GL_UNSIGNED_BYTE, buffers[i]);
        checkGLError();
        spr->animations[i].box = spr->animations[i].box;
        spr->animations[i].box.pos_x    /= (float)f_width;
        spr->animations[i].box.pos_y    /= (float)f_height;
        spr->animations[i].box.size_x   /= (float)f_width * spr->animations[i].length;
        spr->animations[i].box.size_y   /= (float)f_height;
        spr->animations[i].dimensions_x = spr->animations[i].box.size_x / spr->animations[i].length;
        spr->animations[i].dimensions_y = spr->animations[i].box.size_y;
    }

    free(available);
    for(uint16_t i = 0; i < spr->animation_count; ++i)
        free(buffers[i]);
    free(buffers);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    checkGLError();

    spr->atlas->width = f_width;
    spr->atlas->height = f_height;

    xmlFreeDoc(doc);
    return spr;
}

bool save_spriteset_to_resource(spriteset* spr, resource_pair)
{
    stub(false);
}


sprite* create_sprite(spriteset* set)
{
    sprite* spr = malloc(sizeof(sprite));
    spr->source = set;
    spr->handle = 0;

    spr->position = 0;
    spr->playing = false;

    if(set) {
        if(set->animation_count) {
            sprite_set_animation(spr, 0);
        } else
            warn("Creating a sprite for a set with no animations");
    } else {
        warn("Creating a sprite with no set");
    }

    return spr;
}

void destroy_sprite(sprite* spr)
{
    free(spr);
}

bool save_sprite_to_resource(sprite* spr, resource_pair)
{
    stub(false);
}

sprite* load_resource_to_sprite(resource_pair)
{
    stub(0);
}


bool sprite_set_animation(sprite* spr, int16_t index)
{
    nulltest(spr->source);
    if(index >= spr->source->animation_count) {
        error("Can't set animation: Index %d is out of bounds! (%d animations available)", index, spr->source->animation_count);
        return false;
    }
    spr->position = 0;

    spr->handle = &spr->source->animations[index];
    sprite_set_playing(spr, spr->handle->autoplay);

    return true;
}

bool sprite_set_animation_handle(sprite* spr, const char* handle)
{
    return sprite_set_animation(spr, index_by_handle(spr->source, handle));
}

void sprite_set_playing(sprite* spr, bool play)
{
    if(!spr->source || !spr->handle) {
        error("Failed to play sprite animation: No animation to play");
        return;
    }

    spr->playing = play;
}

bool sprite_update(sprite* spr, float delta)
{
    if(!spr->playing)
        return false;

    // TODO: Don't hardcode the framerate
    spr->position += delta * 60;
    if(spr->position >= spr->handle->length) {
        if(spr->handle->autoloop) {
            while(spr->position >= spr->handle->length) {
                spr->position -= spr->handle->length;
            }
            return true;
        } else {
            spr->position = spr->handle->length - 1;
            sprite_set_playing(spr, false);
        }

        return false;
    }

    return true;
}

bool sprite_draw(mat4 camera, mat4 transform, sprite* spr, bool use_dims)
{
    nulltest(spr);
    nulltest(spr->handle);
    return render_quad_subtex(camera, transform, spr->source->atlas, use_dims, spr->handle->box.pos_x + (spr->handle->box.size_x * (int)spr->position), spr->handle->box.pos_y, spr->handle->box.size_x, spr->handle->box.size_y);
}

const char* sprite_get_current_handle(sprite* spr)
{
    return spr->handle->handle;
}

int16_t sprite_get_current_index(sprite* spr)
{
    for(int i = 0; i < spr->source->animation_count; ++i) {
        if(&spr->source->animations[i] == spr->handle)
            return i;
    }
    return -1;
}


int16_t index_by_handle(spriteset* spr, const char* handle)
{
    for(int i = 0; i < spr->animation_count; ++i)
        if(!strcmp(spr->animations[i].handle, handle))
            return i;
    return -1;
}
