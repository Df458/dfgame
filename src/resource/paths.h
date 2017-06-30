#ifndef DF_RESOURCE_PATHS
#define DF_RESOURCE_PATHS
#include "types.h"

#define ASSETS_PREFIX "assets"
#define SAVES_PREFIX "saves"
#define RESOURCE_PATH_DEFAULT_BUFSIZE 1024

void init_base_resource_path(const char* path);

const char* get_base_resource_path();

char* get_resource_path(const char* prefix, const char* suffix, uint16* len);
#define assets_path(s, l) get_resource_path(ASSETS_PREFIX, s, l)
#define saves_path(s, l) get_resource_path(SAVES_PREFIX, s, l)

const char* get_extension(const char* path);
char* get_folder(const char* path);
char* combine_paths(char* a, char* b, bool free);

#endif
