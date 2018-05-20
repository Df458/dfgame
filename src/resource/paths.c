#define LOG_CATEGORY "Resource"

#include "resource/paths.h"

#include "core/check.h"
#include "core/memory/alloc.h"
#include "core/stringutil.h"
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#elif __GNUC__
#include <unistd.h>
#endif

static char* resource_path = NULL;

#ifdef WIN32
#define DELIM '\\'
#else
#define DELIM '/'
#endif

void get_exe_path(char* buf, size_t bufsize) {
#ifdef WIN32
    GetModuleFileName(NULL, buf, bufsize);
    char* c = strrchr(buf, '\\');
    c[1] = 0;
#elif __GNUC__
    ssize_t len = readlink("/proc/self/exe", buf, bufsize - 1);
    if (len != -1) {
        char* c = strrchr(buf, '/');
        c[1] = 0;
    } else {
        error_code();
    }
#else
#error Unsupported platform
#endif
}

void init_base_resource_path(const char* path) {
    if(check_warn(!resource_path, "Redefinition of resource_path from %s to %s", resource_path, path))
        sfree(resource_path);

    resource_path = scalloc(RESOURCE_PATH_DEFAULT_BUFSIZE, sizeof(char));
    if(!path) {
        get_exe_path(resource_path, RESOURCE_PATH_DEFAULT_BUFSIZE);
    } else {
        resource_path = strncpy(resource_path, path, RESOURCE_PATH_DEFAULT_BUFSIZE);
    }
}

void resource_path_free() {
    sfree(resource_path);
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
        data[resource_len] = DELIM;
        pen += resource_len + 1;
    }
    if(prefix) {
        strncpy(data + pen, prefix, prefix_len);
        data[pen + prefix_len] = DELIM;
        pen += prefix_len + 1;
    }
    strncpy(data + pen, suffix, suffix_len);

    if(len)
        *len = length;

    return data;
}

const char* get_extension(const char* path) {
    check_return(path, "Can't get extension, path is null", NULL);

    const char* file_start = strrchr(path, (int)DELIM);

    const char* ext = NULL;
    if(file_start && file_start[1]) {
        ext = strchr(file_start + 2, (int)'.');
    } else if(path[1]) {
        ext = strchr(path + 2, (int)'.');
    }

    if(ext == NULL || strlen(ext) <= 1) {
        return NULL;
    } else {
        return ext + 1;
    }
}

char* get_filename(const char* path, bool keep_extension) {
    check_return(path, "Can't get filename, path is null", NULL);

    const char* file_start = strrchr(path, (int)DELIM);
    if(!file_start) {
        file_start = path;
    } else {
        file_start += 1;
    }

    // If the last slash was at the end of the string, we have no file
    if(!file_start[0]) {
        return NULL;
    }

    if(keep_extension) {
        return nstrdup(file_start);
    }

    const char* file_end = strchr(file_start + 1, '.');

    if(!file_end) {
        return nstrdup(file_start);
    }

    char* str = mscalloc(file_end - file_start + 1, char);
    strncpy(str, file_start, file_end - file_start);

    return str;
}

char* get_folder(const char* path) {
    check_return(path, "Can't get folder, path is null", NULL);

    const char* file_start = strrchr(path, (int)DELIM);
    if(!file_start) {
        char* c = scalloc(2, sizeof(char));
        c[0] = '.';
        return c;
    }

    char* str = scalloc(file_start - path + 1, sizeof(char));
    strncpy(str, path, file_start - path);

    return str;
}

char* combine_paths(char* a, char* b, bool free) {
    check_return(a, "Can't combine paths, one path is null", b);
    check_return(b, "Can't combine paths, one path is null", a);

    size_t last_a = strlen(a) - 1;
    size_t len_b  = strlen(b);
    char* output;
    if(a[last_a] == DELIM) {
        if(b[0] == DELIM) {
            output = scalloc(last_a + len_b + 1, sizeof(char));
            strncat(output, a, last_a);
        } else {
            output = scalloc(last_a + len_b + 2, sizeof(char));
            strncat(output, a, last_a + 1);
        }
        strncat(output, b, len_b);
    } else if(b[0] == DELIM) {
        output = scalloc(last_a + len_b + 2, sizeof(char));
        strncat(output, a, last_a + 1);
        strncat(output, b, len_b);
    } else {
        output = scalloc(last_a + len_b + 3, sizeof(char));
        strncat(output, a, last_a + 1);
        output[last_a + 1] = DELIM;
        output[last_a + 2] = '\0';
        strncat(output, b, len_b);
    }

    if(free) {
        sfree(a);
        sfree(b);
    }

    return output;
}

char* get_relative_base(const char* a, const char* b) {
    check_return(a, "Can't compare paths, one path is null", NULL);
    check_return(b, "Can't compare paths, one path is null", NULL);
    size_t last_a = strlen(a);
    size_t last_b = strlen(b);

    int j = 0;
    for(int i = 0; i < last_a && i < last_b; ++i) {
        if(a[i] != b[i])
            break;

        if(a[i] == '/')
            j = i + 1;
    }

    char* base = mscalloc(j + 1, char);
    if(j > 0)
        strncpy(base, a, j);

    return base;
}
