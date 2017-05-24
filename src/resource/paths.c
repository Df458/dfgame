#define LOG_CATEGORY "Resource"

#include "paths.h"

#include "check.h"
#include "memory/alloc.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char* resource_path = NULL;

// TODO: This code is OS-specific. Adding new build targets will require this
// to be updated
void get_exe_path(char* buf, size_t bufsize) {
    ssize_t len = readlink("/proc/self/exe", buf, bufsize - 1);
    if (len != -1) {
        char* c = strrchr(buf, '/');
        c[1] = 0;
    } else {
        error_code();
    }
}

void init_base_resource_path(const char* path) {
    if(check_warn(!resource_path, "Redefinition of resource_path from %s to %s", resource_path, path))
        sfree(resource_path);

    if(!path) {
        resource_path = scalloc(RESOURCE_PATH_DEFAULT_BUFSIZE, sizeof(char));
        get_exe_path(resource_path, RESOURCE_PATH_DEFAULT_BUFSIZE);
    } else
        resource_path = strndup(path, RESOURCE_PATH_DEFAULT_BUFSIZE);
}

const char* get_base_resource_path() {
    return resource_path;
}

char* get_resource_path(const char* prefix, const char* suffix, uint16* len) {
    check_return(suffix, "Trying to load a resource without a suffix", NULL);

    uint16 prefix_len = prefix ? strlen(prefix) : 0;
    uint16 suffix_len = strlen(suffix);
    uint16 resource_len = resource_path ? strlen(resource_path) : 0;
    uint16 length = prefix_len + suffix_len + (prefix ? 1 : 0);
    if(resource_path)
        length += resource_len + 1;

    char* data = scalloc(length + 1, sizeof(char));

    uint16 pen = 0;
    if(resource_path) {
        strncpy(data, resource_path, resource_len);
        data[resource_len] = '/';
        pen += resource_len + 1;
    }
    if(prefix) {
        strncpy(data + pen, prefix, prefix_len);
        data[pen + prefix_len] = '/';
        pen += prefix_len + 1;
    }
    strncpy(data + pen, suffix, suffix_len);

    if(len)
        *len = length;

    return data;
}

const char* get_extension(const char* path) {
    const char* ext = strrchr(path, (int)'.');
    if(ext == NULL || strlen(ext) <= 1)
        return NULL;
    else
        return ext + 1;
}
