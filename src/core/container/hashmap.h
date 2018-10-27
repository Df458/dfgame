#ifndef DF_CORE_HASHMAP
#define DF_CORE_HASHMAP

#include "core/container/array.h"
#include "core/container/container_common.h"
#include "core/container/delegate.h"
#include "core/crc.h"

typedef struct hashmap_key {
    void* key;
    byte* value;
    uint16 size;
    hash_value hash;
} hashmap_key;

typedef struct hashmap {
    uint16 value_size;
    bool autofree;
    container_index length;
    container_index buckets;

    array* keys;
}* hashmap;
typedef struct hashmap_iter {
    container_index index;
    array_iter bucket_iter;
    int8 increment;
    const void* key;
    void* value;
    bool is_valid;
} hashmap_iter;
#define hashmap_iter_key(iter, type) ((type)iter.key)
#define hashmap_iter_value(iter, type) (*(type*)iter.value)

// Creates a new hashmap with a predefined number of buckets of size size.
#define hashmap_mnew(type, buckets) hashmap_new(sizeof(type), buckets)
hashmap hashmap_new(uint16 value_size, container_index buckets);

// Creates a new hashmap with a predefined number of buckets of size size.
// This hashmap will manage any pointers to keys that it receives.
hashmap hashmap_new_autofree(uint16 value_size, container_index buckets);

// Frees the hashmap, and sets map to NULL to make it harder to double-free.
#define hashmap_free(m) { _hashmap_free(m); m = NULL; }

// Frees the hashmap. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _hashmap_free(hashmap m);

// Returns the number of actual members stored in this hashmap.
container_index hashmap_get_length(hashmap m);

// Returns the number of buckets in this hashmap.
container_index hashmap_get_buckets(hashmap m);

// Inserts a new value mapped to hash_key, replacing any existing value.
// Returns true if a value was replaced.
#define hashmap_insert(m, k, d) ( _hashmap_insert(m, k, &d, sizeof(d)) )
#define hashmap_insertp(m, k, d) ( _hashmap_insert(m, k, d, sizeof(*d)); )
bool _hashmap_insert(hashmap m, hash_key, void* value, uint16 size);

// Inserts a new value mapped to hash_key if it doesn't exist yet.
// If it does, cancel the operation and return false.
#define hashmap_try_insert(a, k, d) ( _hashmap_try_insert(a, k, &d, sizeof(d)) )
#define hashmap_try_insertp(a, k, d) ( _hashmap_try_insert(a, k, d, sizeof(*d)) )
bool _hashmap_try_insert(hashmap m, hash_key, void* value, uint16 size);

// Returns true if a value mapped to hash_key exists in this hashmap, or false
// if it doesn't.
bool hashmap_contains_key(hashmap m, hash_key);

// Returns true if value is mapped to a key in this hashmap, or false if it isn't.
bool hashmap_contains_value(hashmap m, void* value, equality_predicate p, void* user);

// Removes the value mapped to hash_key, returning true if it succeeds.
bool hashmap_remove(hashmap m, hash_key);

// Removes the first matching instance of a value, using predicate p to check
// the data and returning true if it succeeds.
bool hashmap_remove_value(hashmap m, void* value, equality_predicate p, void* user);

// Removes the element pointed to by it from this hashmap
void hashmap_remove_iter(hashmap m, hashmap_iter* it);

// Returns the value mapped to hash_key, or NULL if no value is mapped to it.
void* hashmap_get(hashmap m, hash_key);

// Returns the first matching key that a value is mapped to, using predicate p
// to check the data. Returns NULL if the value is not found
const void* hashmap_get_key(hashmap m, void* value, equality_predicate p, void* user);

// Gets an iterator to the 'start' of the hashmap
hashmap_iter hashmap_get_start(hashmap m);

// Gets an iterator to the 'end' of the hashmap
hashmap_iter hashmap_get_end(hashmap m);

// Advances an iterator to the next spot in the hashmap
void hashmap_get_next(hashmap m, hashmap_iter* i);

// Advances an iterator to the previous spot in the hashmap
void hashmap_get_prev(hashmap m, hashmap_iter* i);

// Helper macro for iterating every pair in a hashmap sequentially
#define hashmap_foreach(hashmap, name) for(hashmap_iter name = hashmap_get_start(hashmap); name.is_valid; hashmap_get_next(hashmap, &name))
#define hashmap_rforeach(hashmap, name) for(hashmap_iter name = hashmap_get_end(hashmap); name.is_valid; hashmap_get_next(hashmap, &name))

// Calls d on each pair in the hashmap. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void hashmap_foreachd(hashmap a, foreach_delegate d, void* user);

#endif // DF_CORE_HASHMAP
