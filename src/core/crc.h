#ifndef DF_CORE_CRC
#define DF_CORE_CRC

#include "types.h"

// Converts data into its respective CRC32 hash.
uint32 crc32(const byte* data, uint16 size);

#endif // DF_CORE_CRC
