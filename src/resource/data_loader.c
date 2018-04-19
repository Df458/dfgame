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
    size_t read = fread(filedata, sizeof(byte), filesize - 1, file);
    check_error(read == filesize - 1, "Failed to read all bytes from %s: %ld/%ld bytes read.", path, read, filesize);
    filedata[filesize - 1] = '\0';

    if(len)
        *len = filesize;

    fclose(file);
    return filedata;
}
