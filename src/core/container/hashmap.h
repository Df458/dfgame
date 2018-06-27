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

// Frees the hashmap and all pointers contained within it, and sets map to NULL
// to make it harder to double-free.
#define hashmap_free_deep(map) { _hashmap_free_deep(map); map = NULL; }

// Frees the hashmap and all pointers contained within it. NOTE: Don't call
// this function. Use the macro without the leading _ instead, as it also NULLs
// your pointer.
void _hashmap_free_deep(hashmap map);

// Returns the number of items stored in map.
uint16 hashmap_size(hashmap map);

// Sets the value mapped to hash_key, creating a new entry if necessary.
// Setting value to NULL will automatically unmap hash_key.
void hashmap_set(hashmap map, hash_key, void* value);

void hashmap_copyset(hashmap map, hash_key, void* value, uint32 size);

// Returns whether or not hash_key is present in the map
bool hashmap_has_key(hashmap map, hash_key);

// Returns the value mapped to hash_key. Returns NULL if hash_key is not
// mapped.
void* hashmap_get(hashmap map, hash_key);

// Returns the first value matching predicate p. Returns NULL if no values
// match.
void* hashmap_get_value(hashmap map, void* data, equality_predicate p, void* user);

// Removes the entry at has_key in the map.
void hashmap_remove(hashmap map, hash_key);

// Calls d on each object in the map. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void hashmap_foreach(hashmap map, foreach_delegate d, void* user);

// Returns an array containing all values
void** hashmap_to_array(hashmap map);

#endif // DF_CORE_HASHMAP
