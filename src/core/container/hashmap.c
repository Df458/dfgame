// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "core/container/hashmap.h"

#include "core/memory/alloc.h"
#include "core/check.h"
#include "core/container/array.h"
#include "core/container/container_common.h"
#include "core/container/delegate.h"
#include "core/crc.h"
#include "core/power.h"

// Private Functions

// Gets the index of key in a given hashmap bucket
container_index hashmap_find_key_index(array bucket, hash_value key) {
    array_foreach(bucket, it) {
        hashmap_key check = array_iter_data(it, hashmap_key);

        if(check.hash == key) {
            return it.index;
        }
    }

    return CONTAINER_INDEX_INVALID;
}

// Searches for value in the given hashmap bucket using predicate p
container_index hashmap_find_value_index(array bucket, void* value, equality_predicate p, void* user) {
        array_foreach(bucket, it) {
            if(p(array_iter_data(it, hashmap_key).value, value, user)) {
                return it.index;
            }
        }

    return CONTAINER_INDEX_INVALID;
}

// Removes the data at position key in a given bucket
void hashmap_remove_key(hashmap m, container_index bucket, container_index key) {
    m->length--;
    hashmap_key* hk = array_get(m->keys[bucket], key);
    if(m->autofree) {
        sfree(hk->key);
    }
    sfree(hk->value);
    array_remove_at(m->keys[bucket], key);
}

// Inserts a new key into a given bucket
void hashmap_insert_key(hashmap m, container_index bucket, hash_key, hash_value hash, byte* data) {
    m->length++;
    hashmap_key key = (hashmap_key) {
        .key = hash_data,
        .value = mscalloc(m->value_size, byte),
        .size = hash_data_size,
        .hash = hash,
    };
    memcpy(key.value, data, m->value_size);
    array_add(m->keys[bucket], key);
}

// Replaces the data at position keu in a given bucket
void hashmap_replace_key(hashmap m, container_index bucket, container_index key, byte* value) {
    hashmap_key* key_data = (hashmap_key*)array_get(m->keys[bucket], key);
    memcpy(key_data->value, value, m->value_size);
}

// ----------------------------------------------------------------------------

// Creates a new hashmap with a predefined number of buckets of size size.
hashmap hashmap_new(uint16 value_size, container_index buckets) {
    check_return(value_size > 0, "Array values must have a size", NULL);

    hashmap m = msalloc(struct hashmap);

    m->autofree = false;
    m->value_size = value_size;
    m->length = 0;
    m->buckets = buckets;

    m->keys = mscalloc(m->buckets, hashmap_key*);

    for(container_index i = 0; i < buckets; ++i) {
        m->keys[i] = array_mnew(hashmap_key, 1);
    }

    return m;
}

// Creates a new hashmap with a predefined number of buckets of size size.
// This hashmap will manage any pointers to keys that it receives.
hashmap hashmap_new_autofree(uint16 value_size, container_index buckets) {
    hashmap m = hashmap_new(value_size, buckets);

    if(m != NULL) {
        m->autofree = true;
    }

    return m;
}

// Frees the hashmap. NOTE: Don't call this function. Use the macro without
// the leading _ instead, as it also NULLs your pointer.
void _hashmap_free(hashmap m) {
    check_return(m != NULL, "Hashmap is NULL", );

    for(container_index i = 0; i < m->buckets; ++i) {
        array_foreach(m->keys[i], it) {
            hashmap_key hk = array_iter_data(it, hashmap_key);
            sfree(hk.value);

            if(m->autofree) {
                sfree(hk.key);
            }
        }

        array_free(m->keys[i]);
    }
    sfree(m->keys);

    sfree(m);
}

// Returns the number of actual members stored in this hashmap.
container_index hashmap_get_length(hashmap m) {
    check_return(m != NULL, "Hashmap is NULL", 0);

    return m->length;
}

// Returns the number of buckets in this hashmap.
container_index hashmap_get_buckets(hashmap m) {
    check_return(m != NULL, "Hashmap is NULL", 0);

    return m->buckets;
}

// Inserts a new value mapped to hash_key, replacing any existing value.
// Returns true if a value was replaced.
bool _hashmap_insert(hashmap m, hash_key, void* value, uint16 size) {
    check_return(m != NULL, "Hashmap is NULL", false);
    check_return(value != NULL, "Value is NULL", false);
    check_return(m->value_size == size, "Hashmap value size mismatch (%d != %d)", false, m->value_size, size);

    hash_value crc = make_crc();
    container_index index = crc % m->buckets;
    container_index old_index = hashmap_find_key_index(m->keys[index], crc);

    // If not replacing a value, increment the length
    if(old_index != CONTAINER_INDEX_INVALID) {
        if(m->autofree) {
            sfree(hash_data);
        }
        hashmap_replace_key(m, index, old_index, value);
        return true;
    }

    // Set the key
    hashmap_insert_key(m, index, hash_data, hash_data_size, crc, value);

    return false;
}

// Inserts a new value mapped to hash_key if it doesn't exist yet.
// If it does, cancel the operation and return false.
bool _hashmap_try_insert(hashmap m, hash_key, void* value, uint16 size) {
    check_return(m != NULL, "Hashmap is NULL", false);
    check_return(value != NULL, "Value is NULL", false);
    check_return(m->value_size == size, "Hashmap value size mismatch (%d != %d)", false, m->value_size, size);

    hash_value crc = make_crc();
    container_index index = crc % m->buckets;
    if(hashmap_find_key_index(m->keys[index], crc) != CONTAINER_INDEX_INVALID) {
        return false;
    }

    // Set the key
    hashmap_insert_key(m, index, hash_key_data, crc, value);

    return true;
}

// Returns true if a value mapped to hash_key exists in this hashmap, or false
// if it doesn't.
bool hashmap_contains_key(hashmap m, hash_key) {
    check_return(m != NULL, "Hashmap is NULL", false);

    hash_value crc = make_crc();
    container_index index = crc % m->buckets;

    return hashmap_find_key_index(m->keys[index], crc) != CONTAINER_INDEX_INVALID;
}

// Returns true if value is mapped to a key in this hashmap, or false if it isn't.
bool hashmap_contains_value(hashmap m, void* value, equality_predicate p, void* user) {
    check_return(m != NULL, "Hashmap is NULL", false);

    for(container_index i = 0; i < m->buckets; ++i) {
        if(hashmap_find_value_index(m->keys[i], value, p, user) != CONTAINER_INDEX_INVALID) {
            return true;
        }
    }
    return false;
}

// Removes the value mapped to hash_key, returning true if it succeeds.
bool hashmap_remove(hashmap m, hash_key) {
    check_return(m != NULL, "Hashmap is NULL", false);

    hash_value crc = make_crc();
    container_index index = crc % m->buckets;
    container_index found_index = hashmap_find_key_index(m->keys[index], crc);

    if(found_index == CONTAINER_INDEX_INVALID) {
        return false;
    }

    hashmap_remove_key(m, index, found_index);

    return true;
}

// Removes the first matching instance of a value, using predicate p to check
// the data and returning true if it succeeds.
bool hashmap_remove_value(hashmap m, void* value, equality_predicate p, void* user) {
    check_return(m != NULL, "Hashmap is NULL", false);

    for(container_index i = 0; i < m->buckets; ++i) {
        container_index j = hashmap_find_value_index(m->keys[i], value, p, user);
        if(j != CONTAINER_INDEX_INVALID) {
            hashmap_remove_key(m, i, j);
            return true;
        }
    }
    return false;
}

// Removes the element pointed to by it from this hashmap
void hashmap_remove_iter(hashmap m, hashmap_iter* it) {
    check_return(m, "Hashmap is NULL", );
    check_return(it && it->is_valid, "Attempting to access a hashmap with an invalid iterator", );

    hashmap_key hk = array_iter_data(it->bucket_iter, hashmap_key);
    if(m->autofree) {
        sfree(hk.key);
    }
    sfree(hk.value);
    array_remove_iter(m->keys[it->index], &it->bucket_iter);
}

// Returns the value mapped to hash_key, or NULL if no value is mapped to it.
void* hashmap_get(hashmap m, hash_key) {
    check_return(m != NULL, "Hashmap is NULL", NULL);

    hash_value crc = make_crc();
    container_index index = crc % m->buckets;
    container_index found_index = hashmap_find_key_index(m->keys[index], crc);

    check_return(found_index != CONTAINER_INDEX_INVALID, "No value mapped to key 0x%x", NULL, hash_data);

    return ((hashmap_key*)array_get(m->keys[index], found_index))->value;
}

// Returns the first matching key that a value is mapped to, using predicate p
// to check the data. Returns NULL if the value is not found
const void* hashmap_get_key(hashmap m, void* value, equality_predicate p, void* user) {
    check_return(m != NULL, "Hashmap is NULL", NULL);

    for(container_index i = 0; i < m->buckets; ++i) {
        container_index j = hashmap_find_value_index(m->keys[i], value, p, user);
        if(j != CONTAINER_INDEX_INVALID) {
            return ((hashmap_key*)array_get(m->keys[i], j))->key;
        }
    }
    return NULL;
}

// Gets an iterator to the 'start' of the hashmap
hashmap_iter hashmap_get_start(hashmap m) {
    check_return(m != NULL, "Hashmap is NULL", (hashmap_iter) {0});
    if(m->length == 0) {
        return (hashmap_iter) {0};
    }

    for(container_index i = 0; i < m->buckets; ++i) {
        array_iter iter = array_get_start(m->keys[i]);

        if(iter.is_valid) {
            hashmap_key key = array_iter_data(iter, hashmap_key);

            return (hashmap_iter) {
                .index = i,
                .bucket_iter = iter,
                .increment = 1,
                .key = key.key,
                .value = key.value,
                .is_valid = true
            };
        }
    }

    return (hashmap_iter) {0};
}

// Gets an iterator to the 'end' of the hashmap
hashmap_iter hashmap_get_end(hashmap m) {
    check_return(m != NULL, "Hashmap is NULL", (hashmap_iter) {0});
    if(m->length == 0) {
        return (hashmap_iter) {0};
    }

    for(container_index i = m->buckets - 1; i >= 0; --i) {
        array_iter iter = array_get_end(m->keys[i]);

        if(iter.is_valid) {
            hashmap_key key = array_iter_data(iter, hashmap_key);

            return (hashmap_iter) {
                .index = i,
                .bucket_iter = iter,
                .increment = -1,
                .key = key.key,
                .value = key.value,
                .is_valid = true
            };
        }
    }

    return (hashmap_iter) {0};
}

// Advances an iterator to the next spot in the hashmap
void hashmap_get_next(hashmap m, hashmap_iter* i) {
    check_return(i != NULL && i->is_valid, "Attempting to advance an invalid iterator", );

    if(i->bucket_iter.is_valid) {
        array_get_next(m->keys[i->index], &i->bucket_iter);
    }

    if(i->bucket_iter.is_valid) {
        hashmap_key key = array_iter_data(i->bucket_iter, hashmap_key);
        i->key = key.key;
        i->value = key.value;

        return;
    }

    i->index += i->increment;
    for(; i->index < m->buckets; i->index += i->increment) {
        array_iter iter = i->increment > 0 ? array_get_start(m->keys[i->index]) : array_get_end(m->keys[i->index]);

        if(iter.is_valid) {
            hashmap_key key = array_iter_data(iter, hashmap_key);
            i->bucket_iter = iter;
            i->key = key.key;
            i->value = key.value;

            return;
        }
    }

    i->key = NULL;
    i->value = NULL;
    i->is_valid = false;
}

// Advances an iterator to the previous spot in the hashmap
void hashmap_get_prev(hashmap m, hashmap_iter* i) {
    check_return(i != NULL && i->is_valid, "Attempting to advance an invalid iterator", );

    if(i->bucket_iter.is_valid) {
        array_get_prev(m->keys[i->index], &i->bucket_iter);
    }

    if(i->bucket_iter.is_valid) {
        hashmap_key key = array_iter_data(i->bucket_iter, hashmap_key);
        i->key = key.key;
        i->value = key.value;

        return;
    }

    i->index -= i->increment;
    for(; i->index < m->buckets; i->index -= i->increment) {
        array_iter iter = i->increment > 0 ? array_get_end(m->keys[i->index]) : array_get_start(m->keys[i->index]);

        if(iter.is_valid) {
            // Reverse the direction of the iterator since we're going backwards
            iter.increment *= -1;

            hashmap_key key = array_iter_data(i->bucket_iter, hashmap_key);
            i->key = key.key;
            i->value = key.value;

            return;
        }
    }

    i->key = NULL;
    i->value = NULL;
    i->is_valid = false;
}

// Calls d on each pair in the hashmap. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void hashmap_foreachd(hashmap m, foreach_delegate d, void* user) {
    check_return(m != NULL, "Hashmap is NULL", );

    hashmap_foreach(m, it) {
        iter_result res = d(it.value, user);

        // Respond to delete/replace decisions
        if(res.decision % 3 == 1) { // DELETE or BREAK_DELETE
            hashmap_remove_iter(m, &it);
        } else if(res.decision % 3 == 2) { // REPLACE or BREAK_REPLACE
            check_warn(res.replacement_value != NULL, "Can't replace value with NULL");
            if(res.replacement_value != NULL) {
                memcpy(it.value, res.replacement_value, m->value_size);
            }
        }

        // If the decision involves breaking, break
        if(res.decision >= DECISION_BREAK) {
            break;
        }
    }
}
