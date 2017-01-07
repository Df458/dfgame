#ifndef DF_CORE_CRC
#define DF_CORE_CRC

#include "types.h"

// Add this macro to a function to add a hash key as an argument
#define hash_key const byte* hash_data, uint16 hash_data_size

// This macro is used to pass the contents of hash_key to another function
#define hash_key_data hash_data, hash_data_size

// Helper function for passing in hash keys.
// Defaults to using sizeof() for the size,
// but also uses strlen() for char*
#define make_hash_key(data) (byte*)data, _Generic(data,\
    char*: strlen(data),\
    const char*: strlen(data),\
    default: sizeof(data)\
)

// This macro calls crc32 on the arguments provided by hash_key
#define make_crc() crc32(hash_data, hash_data_size)

// Converts data into its respective CRC32 hash.
uint32 crc32(hash_key);

#endif // DF_CORE_CRC
