#ifndef DF_CORE_ARRAY
#define DF_CORE_ARRAY

#include "types.h"
#include "delegate.h"

typedef struct array {
    uint16 member_size;
    uint16 length;
    uint16 alloc_length;
    bool preserve_order;

    byte* data;
}* array;
typedef struct array_iter {
    uint16 index;
    int8 increment;
    void* data;
    bool is_valid;
} array_iter;

// Creates a new array with enough space allocated to hold up to reserve members of size size.
// 0 is a valid size, since it will grow to fit new members.
#define array_mnew(type, reserve) array_new(sizeof(type), reserve)
#define array_mnew_ordered(type, reserve) array_new_ordered(sizeof(type), reserve)
array array_new(uint16 size, uint16 reserve);
array array_new_ordered(uint16 size, uint16 reserve);

// Frees the array, and sets array to NULL to make it harder to double-free.
#define array_free(a) { _array_free(a); a = NULL; }

// Frees the array. NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _array_free(array a);

// Returns the number of actual members stored in this array.
uint16 array_get_length(array a);

// Adds a new member to the end of this array, resizing it if necessary.
#define array_add(a, d) { _array_add(a, &d, sizeof(d)); }
#define array_addp(a, d) { _array_add(a, d, sizeof(*d)); }
void _array_add(array a, void* data, uint16 size);

// Adds a new member to a specified position in this array, resizing it if
// necessary.
void array_insert(array a, void* data, uint16 position);

// Returns true if data is a member of this array, or false if it isn't.
bool array_contains(array a, void* data);

// Returns true if data is a member of this array, or false if it isn't. Uses
// predicate p to check the array for data.
bool array_containsp(array a, void* data, equality_predicate p, void* user);

// Returns the position of data in this array, or INVALID_INDEX if array does
// not contain data.
int32 array_find(array a, void* data);

// Returns the position of data using predicate p to check the array, or
// INVALID_INDEX if array does not contain data.
int32 array_findp(array a, void* data, equality_predicate p, void* user);

// Tries to remove data from this array, returning true if it succeeeds. This
// will remove the first copy of data it finds.
bool array_remove(array a, void* data);

// Tries to remove data from this array, using predicate p to check the data
// and returning true if it succeeeds. This will remove the first copy of data
// it finds.
bool array_removep(array a, void* data, equality_predicate p, void* user);

// Removes the element at position in this array.
void array_remove_at(array a, uint16 position);

// Removes the element pointed to by it from this array
void array_remove_iter(array a, array_iter* it);

// Returns the element at position in this array.
void* array_get(array a, uint16 position);

// Replaces the element at position in this array with data.
void array_set(array a, uint16 position, void* data);

// Removes the element at the end of this array and returns it
void* array_pop(array array);

// Performs a heapsort on array using predicate p for comparison.
void array_sort(array a, comparison_predicate p, void* user);

// Gets an iterator to the start of the array
array_iter array_get_start(array array);

// Gets an iterator to the end of the array
array_iter array_get_end(array array);

// Advances an iterator to the next spot in the array
void array_get_next(array a, array_iter* i);

// Advances an iterator to the previous spot in the array
void array_get_prev(array a, array_iter* i);

// Helper macro for iterating every item in an array sequentially
#define array_foreach(array, name) for(array_iter name = array_get_start(array); name.is_valid; array_get_next(array, &name))
#define array_rforeach(array, name) for(array_iter name = array_get_end(array); name.is_valid; array_get_next(array, &name))

// Calls d on each object in the array. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void array_foreachd(array a, foreach_delegate d, void* user);

#endif // DF_CORE_ARRAY
