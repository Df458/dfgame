#define LOG_CATEGORY "Core"

#include "crc.h"

// Private Functions

// Reverses a uint32
static uint32 crc_reverse(uint32 val) {
    val = ((val & 0x55555555) <<  1) | ((val >>  1) & 0x55555555);
    val = ((val & 0x33333333) <<  2) | ((val >>  2) & 0x33333333);
    val = ((val & 0x0F0F0F0F) <<  4) | ((val >>  4) & 0x0F0F0F0F);
    val = (val << 24) | ((val & 0xFF00) << 8) |
        ((val >> 8) & 0xFF00) | (val >> 24);
    return val;
}

// ----------------------------------------------------------------------------

// Converts data into its respective CRC32 hash.
uint32 df_crc32(hash_key) {
    uint32 crc = 0xffffffff;
    uint32 val;
    for(uint16 i = 0; i < hash_data_size; ++i) {
        val = crc_reverse(hash_data[i]);
        for(uint16 j = 0; j <= 7; ++j) {
            if((int32)(crc ^ val) < 0)
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc <<= 1;
            val <<= 1;
        }
    }

    return crc_reverse(~crc);
}

#undef LOG_CATEGORY
