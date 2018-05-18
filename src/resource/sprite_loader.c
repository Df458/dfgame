#define LOG_CATEGORY "Resource"

#include "sprite_loader.h"

#include "check.h"
#include "memory/alloc.h"
#include "paths.h"
#include "spriteset.h"
#include "texture.h"
#include "texture_loader.h"
#include "resource/xmlutil.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

void read_animation(spriteset set, xmlNodePtr node, const char* path) {
    animation anim = {
        .orient_count = 0,
        .origin = {
            .x = 0,
            .y = 0
        },
        .start_delay = 0,
        .frame_count = 1,
        .fps = 60,
        .texture_id = -1,
        .autoplay = true,
        .autoloop = false,
        .default_on_finish = true,
    };

    xml_property_read(node, "orients", &anim.orient_count);
    xml_property_read(node, "origin_x", &anim.origin.x);
    xml_property_read(node, "origin_y", &anim.origin.y);
    xml_property_read(node, "delay", &anim.start_delay);
    xml_property_read(node, "frame_count", &anim.frame_count);
    xml_property_read(node, "fps", &anim.fps);
    xml_property_read(node, "autoplay", &anim.autoplay);
    xml_property_read(node, "default_on_finish", &anim.default_on_finish);
    xml_property_read(node, "autoloop", &anim.autoloop);

    bool should_keep = false;
    rawtex tex = {0};
    char* filename = NULL;
    if(xml_property_read(node, "file", &filename)) {
        char* combined = combine_paths(get_folder(path), filename, true);
        tex = load_texture_raw(combined);
        sfree(combined);
        anim.texture_box.dimensions.x = tex.width;
        anim.texture_box.dimensions.y = tex.height;
        if(tex.data)
            should_keep = true;
    }

    if(!check_warn(should_keep, "Failed to load animation as it lacks a texture")) {
        xmlChar* a = xmlGetProp(node, (const xmlChar*)"name");
        if(a) {
            spriteset_add_animation(set, anim, tex, (char*)a);
            sfree(a);
        } else {
            spriteset_add_animation(set, anim, tex, "default");
        }
    }
    rawtex_cleanup(&tex);
}

spriteset load_spriteset(const char* path) {
    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    check_return(doc, "Failed to load spriteset at path %s", NULL, path);

    xmlNodePtr root = xmlDocGetRootElement(doc);
    for(; root; root = root->next)
        if(root->type == XML_ELEMENT_NODE && !xmlStrcmp(root->name, (const xmlChar*)"spriteset"))
            break;

    check_return(root != NULL, "Spriteset file %s is invalid", NULL, path);

    spriteset spr = spriteset_new(NULL);

    for(xmlNodePtr node = root->children; node; node = node->next)
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation"))
            read_animation(spr, node, path);

    check_warn(spriteset_get_animation_count(spr), "Spriteset file %s has no animations");

    xmlFreeDoc(doc);
    return spr;
}
