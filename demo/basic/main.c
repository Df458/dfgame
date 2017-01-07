#include "crc.h"
#include "log/log.h"
#include "memory/alloc.h"
#include "container/hashmap.h"

int main(int argc, char** argv) {
    info("argv CRC Hashes:");
    hashmap map = hashmap_new();
    for(int i = 1; i < argc; ++i) {
        info("%d: %s", i, argv[i]);
        uint32 crc = crc32(make_hash_key(argv[i]));
        info("[%d] %s: 0x%x", i - 1, argv[i], crc);
        int* num = salloc(sizeof(int));
        *num = i - 1;
        hashmap_set(map, make_hash_key(argv[i]), num);
        info("%d -> [%s]", *num, argv[i]);
    }

    for(int i = 1; i < argc; ++i) {
        info("%d: %s", i, argv[i]);
        int* val = hashmap_get(map, (byte*)argv[i], strlen(argv[i]));
        if(val != NULL)
            info("[%s] -> %d", argv[i], *val);
        else
            info("val is NULL!", argv[i], *val);
    }

    return 0;
}
