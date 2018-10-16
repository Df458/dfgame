#define LOG_CATEGORY "Resource"

#include "resource/sprite_io.h"

#include "core/check.h"
#include "core/container/delegate.h"
#include "core/container/hashmap.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include "graphics/texture.h"
#include "graphics/texture_atlas.h"
#include "graphics/spriteset.priv.h"
#include "resource/paths.h"
#include "resource/texture_loader.h"
#include "resource/xmlutil.h"

#ifdef enable_gif
#include <gif_lib.h>
#endif
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

typedef struct writer_data {
    const char* path;
    xmlTextWriter* writer;
} writer_data;

#ifdef enable_gif
void copy_frame(animation* anim, rawtex* tex, uint16 frame, uint16 target_frame) {
    uint16 width = tex->width / anim->frame_count;
    for(int y = 0; y < tex->height; ++y) {
        for(int x = 0; x < width; ++x) {
            int index = (y * tex->width) + (x + (frame * width));
            int target_index = (y * tex->width) + (x + (target_frame * width));
            tex->data[target_index * 4 + 0] = tex->data[index * 4 + 0];
            tex->data[target_index * 4 + 1] = tex->data[index * 4 + 1];
            tex->data[target_index * 4 + 2] = tex->data[index * 4 + 2];
            tex->data[target_index * 4 + 3] = tex->data[index * 4 + 3];
        }
    }
}

bool read_animation_from_gif(animation* anim, rawtex* tex) {
    // Open the file
    int error;
    GifFileType* file_type = load_and_slurp_gif(anim->filepath);
    if(!file_type) {
        return false;
    }

    *tex = rawtex_new(file_type->SWidth * file_type->ImageCount, file_type->SHeight, 4);
    anim->frame_count = file_type->ImageCount;
    resalloc(anim->frame_times, anim->frame_count * sizeof(uint16));

    int transparency = -1;

    // Fill the texture with gif data
    int last_undisposed = 0;
    for(int i = 0; i < file_type->ImageCount; ++i) {
        GifImageDesc desc = file_type->SavedImages[i].ImageDesc;
        GifByteType* raster = file_type->SavedImages[i].RasterBits;

        // One of these maps may be NULL, so we check the local one first
        GifColorType* colors = desc.ColorMap ? desc.ColorMap->Colors : file_type->SColorMap->Colors;

        GraphicsControlBlock block;
        if(gif_read_gcb(file_type, i, &block)) {
            transparency = block.TransparentColor;
            anim->frame_times[i] = block.DelayTime * 10;
            if(block.DisposalMode == DISPOSE_DO_NOT) {
                last_undisposed = i;
                // Copy the previous frame
                if(i > 0) {
                    copy_frame(anim, tex, i - 1, i);
                }
            } else if(block.DisposalMode == DISPOSE_PREVIOUS) {
                // Copy the last frame to not be disposed frame in
                if(i > 0) {
                    copy_frame(anim, tex, last_undisposed, i);
                }
            } else {
                // Fill with background color
                for(int y = 0; y < file_type->SHeight; ++y) {
                    for(int x = 0; x < file_type->SWidth; ++x) {
                        int index = (y * tex->width) + (x + (i * file_type->SWidth));
                        int color = file_type->SBackGroundColor;
                        if(color != transparency) {
                            tex->data[index * 4 + 0] = colors[color].Red;
                            tex->data[index * 4 + 1] = colors[color].Green;
                            tex->data[index * 4 + 2] = colors[color].Blue;
                            tex->data[index * 4 + 3] = 0xff;
                        }
                    }
                }
            }
        }

        for(int y = 0; y < desc.Height; ++y) {
            for(int x = 0; x < desc.Width; ++x) {
                int index = ((tex->height - y - desc.Top - 1) * tex->width) + (x + desc.Left + (i * file_type->SWidth));
                int color = raster[y * desc.Width + x];
                if(color != transparency) {
                    tex->data[index * 4 + 0] = colors[color].Red;
                    tex->data[index * 4 + 1] = colors[color].Green;
                    tex->data[index * 4 + 2] = colors[color].Blue;
                    tex->data[index * 4 + 3] = 0xff;
                }
            }
        }
    }

    DGifCloseFile(file_type, &error);

    return true;
}
#endif

void read_animation(spriteset set, xmlNodePtr node, const char* path) {
    animation anim = {
        .orient_count = 1,
        .origin = {
            .x = 0,
            .y = 0
        },
        .frame_count = 1,
        .frame_times = mscalloc(1, uint16),
        .total_time = 0,
        .texture_id = ARRAY_INDEX_INVALID,
        .autoplay = true,
        .autoloop = false,
        .default_on_finish = true,

        .filepath = NULL,
        .name = NULL,
    };


    bool should_keep = false;
    rawtex tex = {0};
    char* filename = NULL;
    if(xml_property_read(node, "file", &filename)) {
        anim.filepath = combine_paths(get_folder(path), filename, true);

        // Override for loading animation data from gif files
#ifdef enable_gif
        const char* ext = get_extension(anim.filepath);
        if(!strcmp(ext, "gif")) {
            read_animation_from_gif(&anim, &tex);
            should_keep = true;
        }
#endif

        if(!should_keep) {
            tex = load_texture_raw(anim.filepath);
            anim.texture_box.dimensions.x = tex.width;
            anim.texture_box.dimensions.y = tex.height;
            if(tex.data)
                should_keep = true;
        }
    }

    xml_property_read(node, "orients", &anim.orient_count);
    xml_property_read(node, "origin_x", &anim.origin.x);
    xml_property_read(node, "origin_y", &anim.origin.y);
    xml_property_read(node, "frame_count", &anim.frame_count);
    xml_property_read(node, "frame_delay", &anim.default_frame_time);
    xml_property_read(node, "autoplay", &anim.autoplay);
    xml_property_read(node, "autoloop", &anim.autoloop);
    xml_property_read(node, "default_on_finish", &anim.default_on_finish);

    if(anim.default_frame_time == 0) {
        anim.default_frame_time = 16;
    }

    for(xmlNodePtr child = xml_match_name(node->children, "frame"); child; child = xml_match_name(child->next, "frame")) {
        uint16 id = UINT16_MAX;
        uint16 time = 0;
        xml_property_read(child, "id", &id);
        xml_property_read(child, "delay", &time);

        if(id < anim.frame_count && time != 0) {
            anim.frame_times[id] = time;
        }
    }

    animation_calculate_total_time(&anim);

    if(!check_warn(should_keep, "Failed to load animation as it lacks a texture")) {
        if(!xml_property_read(node, "name", &anim.name)) {
            anim.name = nstrdup(ANIMATION_DEFAULT);
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
    xml_property_write(writer, "frame_count", anim->frame_count);
    xml_property_write(writer, "frame_delay", anim->default_frame_time);
    xml_property_write(writer, "autoplay", anim->autoplay);
    xml_property_write(writer, "autoloop", anim->autoloop);
    xml_property_write(writer, "default_on_finish", anim->default_on_finish);

    if(anim->filepath) {
        char* t_path = get_relative_base(((writer_data*)user)->path, anim->filepath);
        xml_property_write(writer, "file", anim->filepath + strlen(t_path));
        sfree(t_path);

        if(nstrcmp(anim->name, ANIMATION_DEFAULT)) {
            xml_property_write(writer, "name", anim->name);
        }

        for(int i = 0; i < anim->frame_count; ++i) {
            if(anim->frame_times[i] != 0) {
                xmlTextWriterStartElement(writer, (xmlChar*)"frame");
                xml_property_write(writer, "id", i);
                xml_property_write(writer, "delay", anim->frame_times[i]);
                xmlTextWriterEndElement(writer);
            }
        }
    }

    xmlTextWriterEndElement(writer);

    return iter_continue;
}

spriteset load_spriteset(const char* path) {
    const char* ext = get_extension(path);
#ifdef enable_gif
    if(!strcmp(ext, "gif"))
        return load_spriteset_from_image(path);
#endif
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
    spr->asset_path = nstrdup(path);

    for(xmlNodePtr node = root->children; node; node = node->next) {
        if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)"animation")) {
            read_animation(spr, node, path);
        }
    }

    check_warn(spriteset_get_animation_count(spr), "Spriteset file %s has no animations", path);

    xmlFreeDoc(doc);
    return spr;
}

spriteset load_spriteset_from_image(const char* path) {
    animation anim;
    rawtex tex;
    check_return(load_animation_from_image(path, &anim, &tex), "", NULL);


    spriteset spr = spriteset_new(NULL);
    spriteset_add_animation(spr, anim, tex);
    spr->asset_path = nstrdup(path);

    return spr;
}

// Load animation and texture data from an image file, for use in a spriteset
bool load_animation_from_image(const char* path, animation* anim, rawtex* tex) {
    *anim = (animation) {
        .orient_count = 1,
        .origin = {
            .x = 0,
            .y = 0
        },
        .frame_count = 1,
        .frame_times = mscalloc(1, uint16),
        .total_time = 0,
        .default_frame_time = 16,
        .texture_id = ARRAY_INDEX_INVALID,
        .autoplay = true,
        .autoloop = false,
        .default_on_finish = false,

        .filepath = nstrdup(path),
        .name = nstrdup(ANIMATION_DEFAULT),
    };

    bool loaded = false;

    // Override for loading animation data from gif files
#ifdef enable_gif
    const char* ext = get_extension(anim->filepath);
    if(!strcmp(ext, "gif")) {
        read_animation_from_gif(anim, tex);
        loaded = true;
    }
#endif

    animation_calculate_total_time(anim);

    if(!loaded) {
        *tex = load_texture_raw(path);
        if(check_error(tex->asset_path, "Failed to load sprite texture", NULL)) {
            sfree(anim->filepath);
            sfree(anim->frame_times);
            sfree(anim->name);
            *anim = (animation){0};

            return false;
        }
    }

    return true;
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
    array_foreachd(set->animations, write_animation, &data);

    xmlTextWriterEndElement(writer);
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}
