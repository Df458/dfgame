#define LOG_CATEGORY "Core"

#include "hashmap.h"

#include "log/log.h"
#include "alloc.h"
#include "crc.h"
#include "power.h"
#include <assert.h>

// Struct Definitions

typedef struct hashtree {
    uint16  alloc_size;
    uint16  size;
    uint32* keys;
    void**  data;
} hashtree;

typedef struct hashmap {
    hashtree buckets[HASHMAP_BUCKET_COUNT];
    uint16 size;
}* hashmap;

// ----------------------------------------------------------------------------

// Private Functions

int32 hashmap_bucket_index_of(hashmap map, uint32 crc) {
    uint16 bucket_index = crc % HASHMAP_BUCKET_COUNT;

    for(int32 i = 0; i < map->buckets[bucket_index].size; ++i) {
        if(map->buckets[bucket_index].keys[i] == crc) {
            return i;
        }
    }

    return INVALID_INDEX;
}

void hashmap_bucket_remove(hashmap map, uint32 crc) {
    int32 id = hashmap_bucket_index_of(map, crc);

    if(id != INVALID_INDEX) {
        for(int i = map->buckets[crc % HASHMAP_BUCKET_COUNT].size; i != id; --i) {
            map->buckets[crc % HASHMAP_BUCKET_COUNT].keys[i - 1] = map->buckets[crc % HASHMAP_BUCKET_COUNT].keys[i];
            map->buckets[crc % HASHMAP_BUCKET_COUNT].data[i - 1] = map->buckets[crc % HASHMAP_BUCKET_COUNT].data[i];
        }
        map->buckets[crc % HASHMAP_BUCKET_COUNT].size--;
        map->size--;
    }
}

// ----------------------------------------------------------------------------

// Creates a new hashmap.
hashmap hashmap_new() {
    hashmap new_map = salloc(sizeof(struct hashmap));

    new_map->size = 0;

    // Initialize the buckets
    for(int i = 0; i < HASHMAP_BUCKET_COUNT; ++i) {
        new_map->buckets[i].alloc_size = 2;
        new_map->buckets[i].size = 0;
        new_map->buckets[i].keys = scalloc(2, sizeof(uint32));
        new_map->buckets[i].data = scalloc(2, sizeof(void*));
    }

    return new_map;
}

// Frees the hashmap. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _hashmap_free(hashmap map) {
    if(map) {
        // Free all buckets
        for(int i = 0; i < HASHMAP_BUCKET_COUNT; ++i) {
            sfree(map->buckets[i].keys);
            sfree(map->buckets[i].data);
        }
    }

    sfree(map);
}

// Frees the hashmap and all pointers contained within it. NOTE: Don't call
// this function. Use the macro without the leading _ instead, as it also NULLs
// your pointer.
void _hashmap_free_deep(hashmap map) {
    if(map) {
        // Free all buckets
        for(int i = 0; i < HASHMAP_BUCKET_COUNT; ++i) {
            sfree(map->buckets[i].keys);
            for(int j = 0; j < map->buckets[i].size; ++j)
                sfree(map->buckets[i].data[j]);
            sfree(map->buckets[i].data);
        }
    }

    sfree(map);
}

// Returns the number of items stored in map.
uint16 hashmap_size(hashmap map) {
    return map->size;
}

// Sets the value mapped to hash_key, creating a new entry if necessary.
// Setting value to NULL will automatically unmap hash_key.
void hashmap_set(hashmap map, hash_key, void* value) {
    // If we're mapping NULL, remove the key.
    if(value == NULL)
        hashmap_remove(map, hash_key_data);

    uint32 crc = make_crc();
    uint16 bucket_index = crc % HASHMAP_BUCKET_COUNT;

    // Search for an existing key, and replace the value if it's found.
    // Otherwise, add the key-value pair to the bucket.
    int32 index = hashmap_bucket_index_of(map, crc);
    if(index != INVALID_INDEX) {
        map->buckets[bucket_index].data[index] = value;
    } else {
        // Resize the bucket if it's full.
        if(map->buckets[bucket_index].size == map->buckets[bucket_index].alloc_size) {
            map->buckets[bucket_index].alloc_size = next_power_of_two_16(map->buckets[bucket_index].alloc_size + 1);
            map->buckets[bucket_index].data = resalloc(map->buckets[bucket_index].data, map->buckets[bucket_index].alloc_size * sizeof(void*));
            map->buckets[bucket_index].keys = resalloc(map->buckets[bucket_index].data, map->buckets[bucket_index].alloc_size * sizeof(uint32));
        }

        map->buckets[bucket_index].data[map->buckets[bucket_index].size] = value;
        map->buckets[bucket_index].keys[map->buckets[bucket_index].size] = crc;
        map->buckets[bucket_index].size++;
        map->size++;
    }
}

void hashmap_copyset(hashmap map, hash_key, void* value, uint32 size) {
    void* data = salloc(size);

    hashmap_set(map, hash_key_data, memcpy(data, value, size));
}

// Returns whether or not hash_key is present in the map
bool hashmap_has_key(hashmap map, hash_key) {
    uint32 crc = make_crc();

    return hashmap_bucket_index_of(map, crc) != INVALID_INDEX;
}

// Returns the value mapped to hash_key. Returns NULL is hash_key is not
// mapped.
void* hashmap_get(hashmap map, hash_key) {
    uint32 crc = make_crc();
    int32 index = hashmap_bucket_index_of(map, crc);

    if(index == INVALID_INDEX)
        return NULL;

    return map->buckets[crc % HASHMAP_BUCKET_COUNT].data[index];
}

void* hashmap_get_value(hashmap map, void* data, equality_predicate p, void* user) {
    for(uint16 i = 0; i < HASHMAP_BUCKET_COUNT; ++i)
        for(uint16 j = 0; j < map->buckets[i].size; ++j)
            if(p(map->buckets[i].data[j], data, user))
                return map->buckets[i].data[j];

    return NULL;
}

// Removes the entry at has_key in the map.
void hashmap_remove(hashmap map, hash_key) {
    uint32 crc = make_crc();
    hashmap_bucket_remove(map, crc);
}

// Calls d on each object in the array. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void hashmap_foreach(hashmap map, foreach_delegate d, void* user) {
    for(uint16 i = 0; i < HASHMAP_BUCKET_COUNT; ++i) {
        for(uint16 j = 0; j < map->buckets[i].size; ++j) {
            // TODO: We may want to store key values and pass them into this
            //       delegate as well.
            iter_result res = d(map->buckets[i].data[j], user);

            // Respond to delete/replace decisions
            if(res.decision % 3 == 1) { // DELETE or BREAK_DELETE
                hashmap_bucket_remove(map, map->buckets[i].keys[j]);
                --j;
            } else if(res.decision % 3 == 2) { // REPLACE or BREAK_REPLACE
                map->buckets[i].data[j] = res.replacement_value;
            }

            // If the decision involves breaking, break
            if(res.decision >= DECISION_BREAK)
                return;
        }
    }
}

#undef LOG_CATEGORY
