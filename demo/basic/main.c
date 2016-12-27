#include "core/crc.h"
#include "core/log/log.h"

int main(int argc, char** argv) {
    info("argv CRC Hashes:");
    for(int i = 1; i < argc; ++i) {
        uint32 crc = crc32((byte*)argv[i], strlen(argv[i]));
        info("[%d] %s: 0x%x", i - 1, argv[i], crc);
    }

    return 0;
}
