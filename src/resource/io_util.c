#include "io_util.h"
#include "util.h"

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#ifdef __MINGW32__
#include <windows.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#elif __GNUC__
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Hidden variables
///////////////////////////////////////////////////////////////////////////////

char* resource_path = NULL;

///////////////////////////////////////////////////////////////////////////////
// Hidden functions
///////////////////////////////////////////////////////////////////////////////

void get_exe_path(char* buf, size_t bufsize)
{
#ifdef __MINGW32__
    GetModuleFileName(NULL, buf, bufsize);
    char* c = strrchr(buf, '/');
    c[1] = 0;
#elif __GNUC__
    ssize_t len = readlink("/proc/self/exe", buf, bufsize - 1);
    if (len != -1) {
        char* c = strrchr(buf, '/');
        c[1] = 0;
    } else {
        error_code();
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////

const char* get_base_resource_path()
{
    if(resource_path == NULL) {
        resource_path = calloc(512, sizeof(char));
        get_exe_path(resource_path, 512);
    }

    return resource_path;
}

char* construct_extended_resource_path(resource_pair)
{
    nulltest(resource_name);
    size_t len = strlen(get_base_resource_path()) + strlen(resource_name) + 2;
    char* path = NULL;

    if(resource_location != NULL) {
        len += strlen(resource_location) + 1;
        path = calloc(len, sizeof(char));
        strcat(path, resource_path);
        strcat(path, "/");
        strcat(path, resource_location);
        strcat(path, "/");
        strcat(path, resource_name);
    } else {
        path = calloc(len, sizeof(char));
        strcat(path, resource_path);
        strcat(path, "/");
        strcat(path, resource_name);
    }

    return path;
}

void set_resource_path(const char* path)
{
    nulltest(path);
    if(resource_path != NULL)
        free(resource_path);
    resource_path = strdup(path);
}

void set_resource_path_relative(const char* path)
{
    nulltest(path);
    if(resource_path != NULL)
        free(resource_path);

    resource_path = calloc(512 + strlen(path), sizeof(char));
    get_exe_path(resource_path, 512);
    strcat(resource_path, "/");
    strcat(resource_path, path);
}

FILE* load_resource_file(resource_pair, const char* mode)
{
    char* final_path = construct_extended_resource_path(resource_location, resource_name);
    FILE* file = fopen(final_path, mode);
    bool retry = mode[strlen(mode) - 1] == '!';
    if(!file) {
        fprintf(stderr, "%s (%s, %s) not found\n", final_path, resource_location, resource_name);
        if(retry) {
            fprintf(stderr, "Retrying...\n");
            char* mode2 = strdup(mode);
            mode2[0] = 'w';
            file = fopen(final_path, mode2);
        }

        if(!file) {
            warn("Failed to load file: File not found.");
            return NULL;
        }
    }
    free(final_path);

    return file;
}

unsigned char* load_resource_to_buffer(resource_pair)
{
    FILE* file = load_resource_file(resource_location, resource_name, "rb");
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);
    unsigned char* filedata = calloc(filesize, sizeof(unsigned char));
    fread(filedata, 1, filesize, file);
    filedata[filesize - 1] = '\0';

    fclose(file);
    return filedata;
}

char* load_resource_to_string(resource_pair)
{
    FILE* file = load_resource_file(resource_location, resource_name, "rb");
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);
    char* filedata = calloc(filesize, sizeof(char));
    fread(filedata, 1, filesize, file);
    filedata[filesize - 1] = '\0';

    fclose(file);
    return filedata;
}

char* read_string_from_file(FILE* file)
{
    char* str = 0;
    uint16_t len;
    fread(&len, sizeof(uint16_t), 1, file);
    if(len != 0) {
        str = calloc(len + 1, sizeof(char));
        fread(str, sizeof(char), len, file);
    }
    return str;
}

void write_string_to_file(FILE* file, const char* str)
{
    uint16_t len = 0;
    if(str)
        len = strlen(str);
    fwrite(&len, sizeof(uint16_t), 1, file);
    if(len != 0)
        fwrite(str, sizeof(char), len, file);
}

char* get_unique_resource_name(resource_pair)
{
    char* name = strdup(resource_name);
    char* path = construct_extended_resource_path(resource_location, name);
    const char* extension = strrchr(resource_name, '.');
    // FIXME: Extensionless files fail
    char* resource_prefix = 0;
    if(extension) {
        resource_prefix = calloc(strlen(resource_name) - strlen(extension) + 1, sizeof(char));
        strncpy(resource_prefix, resource_name, strlen(resource_name) - strlen(extension));
    } else {
        resource_prefix = strdup(resource_name);
    }

    uint8_t  digits = 1;
    uint16_t digits_next = 10;
    uint16_t value = 1;
    struct stat dir_stat;

    while (stat(path, &dir_stat) != -1) {
        free(path);
        name = realloc(name, (strlen(resource_name) + 4 + digits) * sizeof(char));
        snprintf(name, strlen(resource_name) + 4 + digits, "%s (%d)%s", resource_prefix, value, extension);
        path = construct_extended_resource_path(resource_location, name);

        ++value;
        if(value > digits_next) {
            ++digits;
            digits_next = pow(10, digits);
        }
    }
    free(path);
    free(resource_prefix);
    return name;
}

bool ensure_directory(const char* path)
{
    struct stat dir_stat = {0};
    if (stat(path, &dir_stat) != -1) {
        return false;
    }

#ifdef __MINGW32__
    mkdir(path);
#elif __GNUC__
    mkdir(path, 0700);
#endif
    return true;
}

char* swap_extension(const char* file_name, const char* file_extension)
{
    const char* ext_ptr = strchr(file_name, (int)'.');
    char* new_name = NULL;
    if(file_extension != NULL) {
        new_name = calloc(strlen(file_name) - strlen(ext_ptr) + strlen(file_extension) + 2, sizeof(char));
        strncpy(new_name, file_name, strlen(file_name) - strlen(ext_ptr));
        strcat(new_name, ".");
        strcat(new_name, file_extension);
    } else {
        new_name = calloc(strlen(file_name) - strlen(ext_ptr) + 1, sizeof(char));
        strncpy(new_name, file_name, strlen(file_name) - strlen(ext_ptr));
    }

    return new_name;
}

const char* get_extension(const char* file_name)
{
    const char* ext = strchr(file_name, (int)'.');
    if(ext == NULL || strlen(ext) <= 1)
        return NULL;
    else
        return ext + 1;
}

void* load_anonymous_resource(resource_pair)
{
    // TODO: Implement this
    fprintf(stderr, "Load called: %s, %s\n", resource_location, resource_name);
    return 0;
}

bool resource_eq(resource_pair, const char* resource2_location, const char* resource2_name)
{
    if(!resource_location) {
        if(resource2_location)
            return false;
    } else {
        if(!resource2_location)
            return false;
        if(strcmp(resource_location, resource2_location) || strcmp(resource_name, resource2_name))
            return false;
    }

    return true;
}

bool resource_exists(resource_pair)
{
    struct stat dir_stat = {0};
    char* path  = construct_extended_resource_path(resource_location, resource_name);
    bool exists = stat(path, &dir_stat) != -1;
    free(path);
    return exists;
}

char* path_to_content_path(const char* path)
{
    const char* rpath = get_base_resource_path();
    if(!rpath || !path)
        return NULL;

    size_t rlen = strlen(rpath);
    if(strncmp(path, rpath, rlen))
        return NULL;

    if(strlen(path) <= rlen)
        return NULL;

    return strdup(path + rlen);
}
