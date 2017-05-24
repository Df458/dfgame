#define LOG_CATEGORY "Resource"

#include "data_loader.h"

#include "check.h"
#include "memory/alloc.h"
#include <stdio.h>

byte* load_data_buffer(const char* path, uint32* len) {
    FILE* file = fopen(path, "r");
    check_return(file, "Can't open file %s: file not found", NULL, path);

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);
    byte* filedata = scalloc(filesize, sizeof(byte));
    fread(filedata, 1, filesize, file);
    filedata[filesize - 1] = '\0';

    if(len)
        *len = filesize;

    fclose(file);
    return filedata;
}
