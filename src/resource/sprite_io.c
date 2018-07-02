#define LOG_CATEGORY "Resource"

#include "resource/sprite_io.h"

#include "core/check.h"
#include "core/container/delegate.h"
#include "core/container/hashmap.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include "graphics/spriteset.h"
#include "graphics/texture.h"
#include "graphics/texture_atlas.h"
#include "resource/paths.h"
#include "resource/texture_loader.h"
#include "resource/xmlutil.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

typedef struct spriteset {
    texture_atlas atlas;
    hashmap animations;

    char* asset_path;
}* spriteset;

typedef struct writer_data {
    const char* path;
    xmlTextWriter* writer;
} writer_data;

void read_animation(spriteset set, xmlNodePtr node, const char* path) {
    animation anim = {
        .orient_count = 1,
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

        .filepath = NULL,
        .name = NULL,
    };

    xml_property_read(node, "orients", &anim.orient_count);
    xml_property_read(node, "origin_x", &anim.origin.x);
    xml_property_read(node, "origin_y", &anim.origin.y);
    xml_property_read(node, "delay", &anim.start_delay);
    xml_property_read(node, "frame_count", &anim.frame_count);
    xml_property_read(node, "fps", &anim.fps);
    xml_property_read(node, "autoplay", &anim.autoplay);
    xml_property_read(node, "autoloop", &anim.autoloop);
    xml_property_read(node, "default_on_finish", &anim.default_on_finish);

    bool should_keep = false;
    rawtex tex = {0};
    char* filename = NULL;
    if(xml_property_read(node, "file", &filename)) {
        anim.filepath = combine_paths(get_folder(path), filename, true);
        tex = load_texture_raw(anim.filepath);
        anim.texture_box.dimensions.x = tex.width;
        anim.texture_box.dimensions.y = tex.height;
        if(tex.data)
            should_keep = true;
    }

    if(!check_warn(should_keep, "Failed to load animation as it lacks a texture")) {
        if(!xml_property_read(node, "name", &anim.name)) {
            anim.name = nstrdup("default");
        }

        spriteset_add_animation(set, anim, tex);
    }
    rawtex_cleanup(&tex);
}

iter_result write_animation(void* a, void* user) {
    animation* anim = (animation*)a;
    xmlTextWriter* writer = ((writer_data*)user)->writer;

    xmlTextWriterStartElement(writer, (xmlChar*)"animation");
    xml_property_write(writer, "orients", anim->orient_count);
    xml_property_write(writer, "origin_x", anim->origin.x);
    xml_property_write(writer, "origin_y", anim->origin.y);
    xml_property_write(writer, "delay", anim->start_delay);
    xml_property_write(writer, "frame_count", anim->frame_count);
    xml_property_write(writer, "fps", anim->fps);
    xml_property_write(writer, "autoplay", anim->autoplay);
    xml_property_write(writer, "autoloop", anim->autoloop);
    xml_property_write(writer, "default_on_finish", anim->default_on_finish);

    char* t_path = get_relative_base(((writer_data*)user)->path, anim->filepath);
    xml_property_write(writer, "file", anim->filepath + strlen(t_path));
    sfree(t_path);

    if(strcmp(anim->name, "default"))
        xml_property_write(writer, "name", anim->name);

    xmlTextWriterEndElement(writer);

    return iter_continue;
}

spriteset load_spriteset(const char* path) {
    const char* ext = get_extension(path);
#ifdef enable_png
    if(!strcmp(ext, "png"))
        return load_spriteset_from_image(path);
#endif
#ifdef enable_jpeg
    if(!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        return load_spriteset_from_image(path);
#endif
#ifdef enable_tga
    if(!strcmp(ext, "tga"))
        return load_spriteset_from_image(path);
#endif
#ifdef enable_tiff
    if(!strcmp(ext, "tif") || !strcmp(ext, "tiff"))
        return load_spriteset_from_image(path);
#endif

    return load_spriteset_from_xml(path);
}

spriteset load_spriteset_from_xml(const char* path) {
    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    check_return(doc, "Failed to load spriteset at path %s", NULL, path);

    xmlNodePtr root = xml_match_name(xmlDocGetRootElement(doc), "spriteset");
    check_return(root != NULL, "Spriteset file %s is invalid", NULL, path);

    spriteset spr = spriteset_new(NULL);

    for(xmlNodePtr node = root->children; node; node = node->next)
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation"))
            read_animation(spr, node, path);

    check_warn(spriteset_get_animation_count(spr), "Spriteset file %s has no animations");

    spr->asset_path = nstrdup(path);

    xmlFreeDoc(doc);
    return spr;
}

spriteset load_spriteset_from_image(const char* path) {
    rawtex tex = load_texture_raw(path);
    check_return(tex.asset_path, "Failed to load sprite texture", NULL);

    spriteset spr = spriteset_new(NULL);

    animation anim = {
        .orient_count = 1,
        .origin = {
            .x = 0,
            .y = 0
        },
        .start_delay = 0,
        .frame_count = 1,
        .fps = 0,
        .texture_id = -1,
        .autoplay = true,
        .autoloop = false,
        .default_on_finish = false,

        .filepath = nstrdup(path),
        .name = nstrdup("default"),
    };

    spriteset_add_animation(spr, anim, tex);

    spr->asset_path = nstrdup(path);

    return spr;
}

void save_spriteset(const char* path, spriteset set) {
    xmlTextWriter* writer = xmlNewTextWriterFilename(path, 0);
    check_return(writer, "Failed to open path %s for writing", , path);

    xmlTextWriterStartDocument(writer, NULL, "ISO-8859-1", NULL);
    xmlTextWriterStartElement(writer, (xmlChar*)"spriteset");

    writer_data data = {
        .path = path,
        .writer = writer,
    };
    hashmap_foreach(set->animations, write_animation, &data);

    xmlTextWriterEndElement(writer);
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}
