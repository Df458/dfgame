#ifndef DF_CORE_HASHMAP
#define DF_CORE_HASHMAP

#include "hashmap.hd"

#include "crc.h"
#include "types.h"
#include "delegate.h"

// Hashmaps place data into a statically-defined number of buckets. Increasing
// this value will improve speed, at the cost of memory.
#define HASHMAP_BUCKET_COUNT 64

// Creates a new hashmap.
hashmap hashmap_new();

// Frees the hashmap, and sets map to NULL to make it harder to double-free.
#define hashmap_free(map) { _hashmap_free(map); map = NULL; }

// Frees the hashmap. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _hashmap_free(hashmap map);

// Returns the number of items stored in map.
uint16 hashmap_size(hashmap map);

// Sets the value mapped to hash_key, creating a new entry if necessary.
// Setting value to NULL will automatically unmap hash_key.
void hashmap_set(hashmap map, hash_key, void* value);

// Returns whether or not hash_key is present in the map
bool hashmap_has_key(hashmap map, hash_key);

// Returns the value mapped to hash_key. Returns NULL is hash_key is not
// mapped.
void* hashmap_get(hashmap map, hash_key);

// Removes the entry at has_key in the map.
void hashmap_remove(hashmap map, hash_key);

// Calls d on each object in the array. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void hashmap_foreach(hashmap map, foreach_delegate d, void* user);

#endif // DF_CORE_HASHMAP