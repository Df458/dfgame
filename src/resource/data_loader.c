#define LOG_CATEGORY "Resource"

#include "data_loader.h"

#include "check.h"
#include "memory/alloc.h"
#include <stdio.h>
#include <zlib.h>

/** @brief Load bytes from a file on disk to a data buffer
 *
 * If the load fails, the return value will be NULL. Otherwise, the data must
 * be freed by the user.
 *
 * @param path The filepath to load from
 * @param len An int pointer that will contain the length of the resulting buffer, in bytes
 */
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

/** @brief Load bytes from a ZLib or GZip compressed file on disk to a data buffer
 *
 * If the load fails, the return value will be NULL. Otherwise, the data must
 * be freed by the user.
 *
 * @param path The filepath to load from
 * @param len An int pointer that will contain the length of the resulting buffer, in bytes
 */
byte* load_data_buffer_compressed(const char* path, uint32* len) {
	FILE* file = fopen(path, "r");
    check_return(file, "Can't open file %s: file not found", NULL, path);

    uint32 have;
    *len = 0;
    ubyte* finalout = NULL;

    ubyte in[DECOMPRESS_BUFFER_SIZE];
    ubyte out[DECOMPRESS_BUFFER_SIZE];

    z_stream strm = {
        .zalloc = Z_NULL,
        .zfree = Z_NULL,
        .opaque = Z_NULL,
        .avail_in = 0,
        .next_in = Z_NULL,
    };

    check_return(inflateInit2(&strm, 32 + MAX_WBITS) == Z_OK, "inflate() failed", NULL, path);
    int ret = Z_STREAM_END;
    do {
        strm.avail_in = fread(in, 1, DECOMPRESS_BUFFER_SIZE, file);
        if (check_error(!ferror(file), "Error reading data from file")) {
            inflateEnd(&strm);
            sfree (finalout);
            *len = 0;

            return NULL;
        }

        if (strm.avail_in == 0)
            break;

        strm.next_in = in;

        do {
            strm.avail_out = DECOMPRESS_BUFFER_SIZE;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            switch (ret) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    error ("Error code 0x%x received", ret);
                    inflateEnd(&strm);
                    sfree (finalout);
                    *len = 0;

                    return NULL;
            }

            have = DECOMPRESS_BUFFER_SIZE - strm.avail_out;
            *len += have;
            if(finalout != NULL) {
                resalloc (finalout, *len * sizeof (ubyte));
            } else {
                finalout = mscalloc(*len, ubyte);
            }

            for(unsigned i = 0; i < have; ++i){
                finalout[i + (*len - have)] = out[i];
            }
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    (void)inflateEnd(&strm);
    fclose(file);

    return (byte*)finalout;
}
