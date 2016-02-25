#include <stdlib.h>
#include <png.h>
#include <string.h>

#include "sprite.h"
#include "util.h"
#include "xml_util.h"

///////////////////////////////////////////////////////////////////////////////
// Hidden structures
///////////////////////////////////////////////////////////////////////////////

typedef struct texture_atlas_box
{
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t size_x;
    uint16_t size_y;
} texture_atlas_box;

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

uint8_t box_contains(texture_atlas_box holder, texture_atlas_box child)
{
    if(holder.size_x >= child.size_x && holder.size_y >= child.size_y)
        return 1;
    return 0;
}

sprite* create_sprite()
{
	sprite* sprite_data = malloc(sizeof(sprite));
    sprite_data->atlas = 0;
    sprite_data->animations = 0;
    sprite_data->animation_count = 0;

    return sprite_data;
}

void destroy_sprite(sprite* spr)
{
    if(spr->atlas) {
            destroy_texture(spr->atlas);
        for(uint8_t i = 0; i < spr->animation_count; ++i)
            free(spr->animations[i].handle);
        free(spr->animations);
    }
    free(spr);
}

// TODO: Add support for paths in sprite files
sprite* load_resource_to_sprite(const char* resource_location, const char* resource_name)
{
    xmlDocPtr doc = load_resource_to_xml(resource_location, resource_name);
    if(!doc)
        return 0;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    for(; root; root = root->next)
        if(root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"sprite"))
            break;
    if(!root)
        return 0;
    sprite* spr = malloc(sizeof(sprite));
    spr->animations = malloc(sizeof(struct animation));
    spr->animation_count = 0;
    png_byte** buffers = malloc(sizeof(png_byte*));
    texture_atlas_box* boxes = malloc(sizeof(texture_atlas_box));
    for(xmlNodePtr node = root->children; node; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation")) {
            spr->animation_count++;
            if(spr->animation_count > 1) {
                spr->animations = realloc(spr->animations, spr->animation_count * sizeof(struct animation));
                buffers = realloc(buffers, spr->animation_count * sizeof(png_byte*));
                boxes = realloc(boxes, spr->animation_count * sizeof(texture_atlas_box));
            }
            spr->animations[spr->animation_count - 1].handle = "idle";
            spr->animations[spr->animation_count - 1].orients = 0;
            spr->animations[spr->animation_count - 1].length = 1;
            spr->animations[spr->animation_count - 1].delay = 1;
            spr->animations[spr->animation_count - 1].loop = 0;
            spr->animations[spr->animation_count - 1].play = 1;
            spr->animations[spr->animation_count - 1].origin_x = 0;
            spr->animations[spr->animation_count - 1].origin_y = 0;
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
                spr->animations[spr->animation_count - 1].loop = strcmp((char*)a, "false") ? 1 : 0;
                free(a);
            }
            a = 0;
            if((a = xmlGetProp(node, (const xmlChar*)"play"))) {
                spr->animations[spr->animation_count - 1].play = strcmp((char*)a, "false") ? 1 : 0;
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
                boxes[spr->animation_count - 1].size_x = w;
                boxes[spr->animation_count - 1].size_y = h;
                boxes[spr->animation_count - 1].pos_x = 0;
                boxes[spr->animation_count - 1].pos_y = 0;
                if(buffers[spr->animation_count - 1])
                    keep = 1;
                free(a);
            }
            if(!keep) {
                spr->animation_count--;
                if(spr->animation_count > 0) {
                    spr->animations = realloc(spr->animations, spr->animation_count * sizeof(struct animation));
                    buffers = realloc(buffers, spr->animation_count * sizeof(png_byte*));
                    boxes = realloc(boxes, spr->animation_count * sizeof(texture_atlas_box));
                }
            }
        }
    }

    if(spr->animation_count == 0) {
        error("Cannot load sprite: No animations found.\n");
        free(spr->animations);
        free(spr);
        free(boxes);
        xmlFreeDoc(doc);
        /*xmlCleanupParser();*/
        return 0;
    }
    // TODO: Sort by width
    // spr up the initial box for the texture
    texture_atlas_box* available = malloc(sizeof(texture_atlas_box));
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
            if(box_contains(available[j], boxes[i]) && (selected < 0 || available[j].size_x * available[j].size_y > available[selected].size_x * available[selected].size_y)) {
                selected = j;
            }
        }
        if(selected == -1) {
            width += 128;
            height += 128;
            avail_length += 2;
            available = realloc(available, avail_length * sizeof(texture_atlas_box));
            available[avail_length - 2].pos_x = width - 128;
            available[avail_length - 2].pos_y = 0;
            available[avail_length - 2].size_x = 128;
            available[avail_length - 2].size_y = height;
            available[avail_length - 1].pos_x = 0;
            available[avail_length - 1].pos_y = height - 128;
            available[avail_length - 2].size_x = width - 128;
            available[avail_length - 2].size_y = 128;
        }
        boxes[i].pos_x = available[selected].pos_x;
        boxes[i].pos_y = available[selected].pos_y;
        if(boxes[i].pos_x + boxes[i].size_x > f_width)
            f_width = boxes[i].pos_x + boxes[i].size_x;
        if(boxes[i].pos_y + boxes[i].size_y > f_height)
            f_height = boxes[i].pos_y + boxes[i].size_y;
        avail_length--;
        if(boxes[i].size_x < available[selected].size_x) {
            avail_length++;
            available[selected].pos_x += boxes[i].size_x;
            available[selected].size_x -= boxes[i].size_x;
            if(boxes[i].size_y < available[selected].size_y) {
                avail_length++;
                available = realloc(available, avail_length * sizeof(texture_atlas_box));
                available[avail_length - 1].pos_x = boxes[i].pos_x;
                available[avail_length - 1].pos_y = boxes[i].pos_y + boxes[i].size_y;
                available[avail_length - 1].size_x = boxes[i].size_x;
                available[avail_length - 1].size_y = available[selected].size_y - boxes[i].size_y;
            }
        } else if(boxes[i].size_y < available[selected].size_y) {
            avail_length++;
            available[selected].pos_y += boxes[i].size_y;
            available[selected].size_y -= boxes[i].size_y;
        } else {
            for(int k = selected + 1; k <= avail_length; ++k) {
                available[k - 1] = available[k];
            }
            available = realloc(available, avail_length * sizeof(texture_atlas_box));
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, f_width, f_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    for(int i = 0; i < spr->animation_count; ++i) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, boxes[i].pos_x, boxes[i].pos_y, boxes[i].size_x, boxes[i].size_y, GL_RGBA, GL_UNSIGNED_BYTE, buffers[i]);
        spr->animations[i].offset_x = (float)boxes[i].pos_x / (float)f_width;
        spr->animations[i].offset_y = (float)boxes[i].pos_y / (float)f_width;
        spr->animations[i].dimensions_x = boxes[i].size_x / spr->animations[i].length;
        spr->animations[i].dimensions_y = boxes[i].size_y;
        spr->animations[i].size_x = (float)boxes[i].size_x / (float)f_width / spr->animations[i].length;
        spr->animations[i].size_y = (float)boxes[i].size_y / (float)f_height;
    }

    free(boxes);
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

    spr->atlas->handle = texture;
    spr->atlas->width = f_width;
    spr->atlas->width = f_height;

    xmlFreeDoc(doc);
    return spr;
}

bool save_sprite_to_resource(sprite* spr, const char* resource_location, const char* resource_name)
{
    stub(false);
}

int8_t index_by_handle(sprite* spr, const char* handle)
{
    for(int i = 0; i < spr->animation_count; ++i)
        if(!strcmp(spr->animations[i].handle, handle))
            return i;
    return -1;
}
