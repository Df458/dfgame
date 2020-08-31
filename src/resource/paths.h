#ifndef DF_RESOURCE_PATHS
#define DF_RESOURCE_PATHS
#include "core/types.h"

#define ASSETS_PREFIX "assets"
#define SAVES_PREFIX "saves"
#define RESOURCE_PATH_DEFAULT_BUFSIZE 1024

void init_base_resource_path(const char* path);
void resource_path_free();

// Get the resource path, initializing it if necessary
const char* get_base_resource_path();

char* get_resource_path(const char* prefix, const char* suffix, uint16* len);
#define assets_path(s, l) get_resource_path(ASSETS_PREFIX, s, l)
#define saves_path(s, l) get_resource_path(SAVES_PREFIX, s, l)

// Convert a path to a resource path if it's relative, or get a copy if it's absolute
char* as_resource_path(const char* path);

// Get the extension of the given filepath
const char* get_extension(const char* path);

// Get the filename of the given path
char* get_filename(const char* path, bool keep_extension);

// Get the folder part of the given path
char* get_folder(const char* path);

// Combine two paths, optionally freeing them
char* combine_paths(char* a, char* b, bool free);

// Get the common base path from which two given paths diverge
char* get_relative_base(const char* a, const char* b);

// Check if the given path is an absolute path or a relative path
bool is_absolute_path(const char* path);

#endif
