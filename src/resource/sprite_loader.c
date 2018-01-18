#define LOG_CATEGORY "Resource"

#include "sprite_loader.h"

#include "check.h"
#include "memory/alloc.h"
#include "paths.h"
#include "spriteset.h"
#include "texture.h"
#include "texture_loader.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

void read_animation(spriteset set, xmlNodePtr node, const char* path) {
    animation anim =  {
        .orient_count = 0,
        .origin = {
            .x = 0,
            .y = 0
        },
        .texture_box = (aabb_2d){ 0 },

        .start_delay = 0,
        .frame_count = 1,
        .fps = 60,

        .texture_id = -1,

        .autoplay = true,
        .autoloop = false,
        .default_on_finish = true,
    };

    xmlChar* a = 0;
    if((a = xmlGetProp(node, (const xmlChar*)"orients"))) {
        anim.orient_count = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"origin_x"))) {
        anim.origin.x = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"origin_y"))) {
        anim.origin.y = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"delay"))) {
        anim.start_delay = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"frame_count"))) {
        anim.frame_count = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"fps"))) {
        anim.fps = atoi((char*)a);
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"autoplay"))) {
        anim.autoplay = strcmp((char*)a, "false");
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"default_on_finish"))) {
        anim.default_on_finish = strcmp((char*)a, "false");
        sfree(a);
    }
    if((a = xmlGetProp(node, (const xmlChar*)"autoloop"))) {
        anim.autoloop = strcmp((char*)a, "false");
        sfree(a);
    }

    bool should_keep = false;
    rawtex tex = (rawtex){0};
    if((a = xmlGetProp(node, (const xmlChar*)"file"))) {
        tex = load_texture_raw(combine_paths(get_folder(path), (char*)a, true));
        anim.texture_box.dimensions.x = tex.width;
        anim.texture_box.dimensions.y = tex.height;
        if(tex.data)
            should_keep = true;
    }

    a = xmlGetProp(node, (const xmlChar*)"name");
    if(check_warn(should_keep, "Failed to load animation as it lacks a texture")) {
        sfree(a);
    } else if(a) {
        spriteset_add_animation(set, anim, tex, (char*)a);
        sfree(a);
    } else {
        spriteset_add_animation(set, anim, tex, "default");
    }
    
    sfree(tex.data);
}

spriteset load_spriteset(const char* path) {
    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    check_return(doc, "Failed to load spriteset at path %s", NULL, path);

    xmlNodePtr root = xmlDocGetRootElement(doc);
    for(; root; root = root->next)
        if(root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"spriteset"))
            break;

    check_return(root, "Spriteset file %s is invalid", NULL, path);

    spriteset spr = spriteset_new();

    for(xmlNodePtr node = root->children; node; node = node->next)
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation"))
            read_animation(spr, node, path);

    check_warn(spriteset_get_animation_count(spr), "Spriteset file %s has no animations");

    xmlFreeDoc(doc);
    return spr;
}
