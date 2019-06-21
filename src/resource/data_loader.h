#ifndef DF_RESOURCE_DATA_LOADER
#define DF_RESOURCE_DATA_LOADER
#include "core/types.h"

#ifndef DECOMPRESS_BUFFER_SIZE
/** The size of the buffer used when decompressing files
 *
 */
#define DECOMPRESS_BUFFER_SIZE 128000
#endif

/** @brief Load bytes from a file on disk to a data buffer
 *
 * If the load fails, the return value will be NULL. Otherwise, the data must
 * be freed by the user.
 *
 * @param path The filepath to load from
 * @param len An int pointer that will contain the length of the resulting buffer, in bytes
 */
byte* load_data_buffer(const char* path, uint32* len);

/** @brief Load bytes from a ZLib or GZip compressed file on disk to a data buffer
 *
 * If the load fails, the return value will be NULL. Otherwise, the data must
 * be freed by the user.
 *
 * @param path The filepath to load from
 * @param len An int pointer that will contain the length of the resulting buffer, in bytes
 */
byte* load_data_buffer_compressed(const char* path, uint32* len);

#endif
