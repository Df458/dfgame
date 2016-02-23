#include "asset_registry.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// Hidden variables
///////////////////////////////////////////////////////////////////////////////

struct resource
{
    char*    path;
    char*    name;
    uint32_t id;
    bool     loaded;
    bool     valid;
    void*    data;
};

struct resource* resource_list = 0;
struct resource* failed_list = 0;
resource_loader  load_func = 0;

static uint32_t next_id = 0;
static uint32_t resource_count = 0;
static uint32_t failed_count = 0;

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

void skip_resource(FILE* file)
{
    fseek(file, 1, SEEK_CUR);
    uint16_t len;
    fread(&len, sizeof(uint16_t), 1, file);
    fseek(file, len, SEEK_CUR);
    fread(&len, sizeof(uint16_t), 1, file);
    fseek(file, len, SEEK_CUR);
}

bool update_resource_definition(uint32_t id)
{
    if(id >= next_id)
        return false;

    FILE* definition_file = load_resource_file(NULL, ".asset_registry", "r+!");
    if(!definition_file)
        return false;

    fseek(definition_file, 0, SEEK_END);
    long file_len = ftell(definition_file);
    rewind(definition_file);
    uint32_t definition_count = 0;
    fread(&definition_count, sizeof(uint32_t), 1, definition_file);
    if(definition_count <= id) {
        rewind(definition_file);
        uint32_t next_in = id + 1;
        fwrite(&next_in, sizeof(uint32_t), 1, definition_file);

        fseek(definition_file, 0, SEEK_END);
        for(uint32_t i = definition_count; i <= id; ++i) {
            fputc(resource_list[i].valid ? 1 : 0, definition_file);
            if(resource_list[i].valid) {
                write_string_to_file(definition_file, resource_list[i].path);
                write_string_to_file(definition_file, resource_list[i].name);
            }
        }
    } else {
        for(int i = 0; i < id; ++i)
            skip_resource(definition_file);
        fpos_t* pos = malloc(sizeof(fpos_t));
        fgetpos(definition_file, pos);
        skip_resource(definition_file);

        size_t data_len = file_len - ftell(definition_file);
        unsigned char* data_buffer = calloc(data_len, sizeof(unsigned char));
        fread(data_buffer, sizeof(unsigned char), data_len, definition_file);
        fsetpos(definition_file, pos);
        free(pos);

        fputc(resource_list[id].valid ? 1 : 0, definition_file);
        if(resource_list[id].valid) {
            write_string_to_file(definition_file, resource_list[id].path);
            write_string_to_file(definition_file, resource_list[id].name);
        }

        fwrite(data_buffer, sizeof(unsigned char), data_len, definition_file);
        free(data_buffer);

        fclose(definition_file);
        return false;
    }

    fclose(definition_file);
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////

bool resources_init(resource_loader loader)
{
    if(loader == 0)
        return false;

    load_func = loader;

    FILE* definition_file = load_resource_file(NULL, ".asset_registry", "r");
    if(!definition_file) {
        create_resource_definition_file();
        definition_file = load_resource_file(NULL, ".asset_registry", "r");
        if(!definition_file)
            return false;
        warn("The asset registry was missing. A new one has been created, but links between assets have been broken.");
    }

    fread(&next_id, sizeof(uint32_t), 1, definition_file);
    info("Resource count: %u\n", next_id);

    resource_count = RESOURCE_BLOCK_COUNT + next_id;
    resource_list = calloc(resource_count, sizeof(struct resource));
    if(!resource_list) {
        fatal("Cannot allocate resource registry");
    }

    for(int i = 0; i < next_id; ++i) {
        char is_valid = fgetc(definition_file);
        resource_list[i].loaded = false;
        if(is_valid) {
            resource_list[i].path  = read_string_from_file(definition_file);
            resource_list[i].name  = read_string_from_file(definition_file);
            resource_list[i].id    = i;
            resource_list[i].valid = resource_exists(resource_list[i].path, resource_list[i].name);
            if(!resource_list[i].valid) {
                ++failed_count;
                failed_list = realloc(failed_list, sizeof(struct resource) * failed_count);
                failed_list[failed_count - 1] = resource_list[i];
            }
        } else {
            resource_list[i].path = 0;
            resource_list[i].name = 0;
            resource_list[i].id    = i;
            resource_list[i].valid = false;
        }
        info("Loaded resource %d:\n%s, %s\n", i, resource_list[i].path, resource_list[i].name);
    }

    if(failed_count > 0)
        warn("%d assets have been moved or deleted since the last load.", failed_count);

    fclose(definition_file);

    return true;
}

void resources_cleanup()
{
    stub();
}

bool create_resource_definition_file()
{
    uint32_t count = 0;
    FILE* asset_registry = load_resource_file(NULL, ".asset_registry", "w");
    if(!asset_registry)
        return false;

    fwrite(&count, sizeof(uint32_t), 1, asset_registry);
    fclose(asset_registry);

    return true;
}

uint32_t get_id_from_resource(const char* resource_location, const char* resource_name, bool should_create)
{
    // TODO: This is extremely slow. This should eventually be replaced with
    //       a tree search, possibly with hashing
    uint32_t i;
    for(i = 0; i < next_id; ++i) {
        if(resource_eq(resource_location, resource_name, resource_list[i].path, resource_list[i].name))
            break;
    }
    if(i == next_id && should_create) {
        // TODO: Check if the paths are valid
        // FIXME: This will technically break if a: realloc fails or
        // b: we run out of ids
        if(next_id >= resource_count) {
            resource_count += RESOURCE_BLOCK_COUNT;
            resource_list = realloc(resource_list, resource_count * sizeof(struct resource));
        }
        if(resource_location != 0)
            resource_list[next_id].path = strdup(resource_location);
        else
            resource_list[next_id].path = 0;
        resource_list[next_id].name   = strdup(resource_name);
        resource_list[next_id].loaded = false;
        resource_list[next_id].valid  = true;
        resource_list[next_id].data   = 0;
        resource_list[next_id].id     = next_id;
        ++next_id;
        update_resource_definition(next_id - 1);
    }

    return i;
}

void* get_data_from_id(uint32_t id)
{
    if(id < next_id && resource_list[id].valid) {
        if(!resource_list[id].loaded) {
            resource_list[id].data = load_func(resource_list[id].path, resource_list[id].name);
            if(resource_list[id].data != 0)
                resource_list[id].loaded = true;
        }
        return resource_list[id].data;
    } else
        return 0;
}

char* get_name_from_id(uint32_t id, bool force_invalid)
{
    if(id < next_id && (resource_list[id].valid || force_invalid)) {
        return strdup(resource_list[id].name);
    } else
        return 0;
}   

char* get_path_from_id(uint32_t id, bool force_invalid)
{
    if(id < next_id && (resource_list[id].valid || force_invalid) && resource_list[id].path != 0) {
        return strdup(resource_list[id].path);
    } else
        return 0;
}

void* get_data_from_resource(const char* resource_location, const char* resource_name)
{
    uint32_t id = get_id_from_resource(resource_location, resource_name, false);
    if(id == UINT32_MAX)
        return 0;
    else
        return get_data_from_id(id);
}

bool move_resource(const char* prev_location, const char* prev_name, const char* next_location, const char* next_name)
{
    uint32_t id = get_id_from_resource(prev_location, prev_name, false);
    return move_id(id, next_location, next_name);
}

void* delete_resource(const char* resource_location, const char* resource_name)
{
    uint32_t id = get_id_from_resource(resource_location, resource_name, false);

    return delete_id(id);
}

bool move_id(uint32_t id, const char* next_location, const char* next_name)
{
    if(id == UINT32_MAX)
        return false;

    struct resource* r = &resource_list[id];
    if(r->path)
        free(r->path);
    if(r->name)
        free(r->name);

    if(next_location)
        r->path = strdup(next_location);
    else
        r->path = 0;
    r->name = strdup(next_name);
    r->valid = true;

    return update_resource_definition(id);
}

void* delete_id(uint32_t id)
{
    if(id == UINT32_MAX)
        return 0;

    struct resource* r = &resource_list[id];
    r->valid = false;
    if(!update_resource_definition(id))
    {
        r->valid = true;
        return 0;
    }
    if(r->path)
        free(r->path);
    if(r->name)
        free(r->name);

    return r->data;
}

uint32_t get_failed_count()
{
    return failed_count;
}

uint32_t* get_failed_ids()
{
    uint32_t* id_list = calloc(failed_count, sizeof(uint32_t));
    for(uint32_t i = 0; i < failed_count; ++i) {
        id_list[i] = failed_list[i].id;
    }
    
    return id_list;
}
