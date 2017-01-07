#ifndef DF_CORE_ARRAY
#define DF_CORE_ARRAY

#include "types.h"
#include "delegate.h"

// Represents an order-preserving auto-resizing array. The order of the contents
// can only be changed manually.
typedef struct sarray* sarray;

// Represents an unsorted auto-resizing array. The order of the contents is
// subject to change, but this structure is more speed efficient than with an
// sarray.
typedef struct uarray* uarray;

// Creates a new array with enough space allocated to hold up to size members.
// 0 is a valid size, since it will grow to fit new members.
sarray sarray_new(uint16 size);
uarray uarray_new(uint16 size);

// Frees the array, and sets array to NULL to make it harder to double-free.
#define array_free(array) _Generic(array,\
    sarray: { _sarray_free(array); array = NULL; },\
    uarray: { _uarray_free(array); array = NULL; },\
)
#define sarray_free { _sarray_free(array); array = NULL; }
#define uarray_free { _uarray_free(array); array = NULL; }

// Frees the array. NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _sarray_free(sarray array);
void _uarray_free(uarray array);

// Returns the number of actual members stored in this array.
#define array_size(array) _Generic(array,\
    sarray: sarray_size(array),\
    uarray: uarray_size(array),\
)
uint16 sarray_size(sarray array);
uint16 uarray_size(uarray array);

// Adds a new member to the end of this array, resizing it if necessary.
#define array_add(array, data) _Generic(array,\
    sarray: sarray_add(array, data),\
    uarray: uarray_add(array, data),\
)
void sarray_add(sarray array, void* data);
void uarray_add(uarray array, void* data);

// Adds a new member to a specified position in this array, resizing it if
// necessary.
#define array_insert(array, data, position) _Generic(array,\
    sarray: sarray_insert(array, data, position),\
    uarray: uarray_insert(array, data, position),\
)
void sarray_insert(sarray array, void* data, uint16 position);
void uarray_insert(uarray array, void* data, uint16 position);

// Returns true if data is a member of this array, or false if it isn't.
#define array_contains(array, data) _Generic(array,\
    sarray: sarray_contains(array, data),\
    uarray: uarray_contains(array, data),\
)
bool sarray_contains(sarray array, void* data);
bool uarray_contains(uarray array, void* data);

// Returns true if data is a member of this array, or false if it isn't. Uses
// predicate p to check the array for data.
#define array_containsp(array, data, user) _Generic(array,\
    sarray: sarray_containsp(array, data, user),\
    uarray: uarray_containsp(array, data, user),\
)
bool sarray_containsp(sarray array, void* data, equality_predicate p, void* user);
bool uarray_containsp(uarray array, void* data, equality_predicate p, void* user);

// Returns the position of data in this array, or INVALID_INDEX if array does
// not contain data.
#define array_find(array, data) _Generic(array,\
    sarray: sarray_find(array, data),\
    uarray: uarray_find(array, data),\
)
int32 sarray_find(sarray array, void* data);
int32 uarray_find(uarray array, void* data);

// Returns the position of data using predicate p to check the array, or
// INVALID_INDEX if array does not contain data.
#define array_findp(array, data, user) _Generic(array,\
    sarray: sarray_findp(array, data, user),\
    uarray: uarray_findp(array, data, user),\
)
int32 sarray_findp(sarray array, void* data, equality_predicate p, void* user);
int32 uarray_findp(uarray array, void* data, equality_predicate p, void* user);

// Tries to remove data from this array, returning true if it succeeeds. This
// will remove the first copy of data it finds.
#define array_remove(array, data) _Generic(array,\
    sarray: sarray_remove(array, data),\
    uarray: uarray_remove(array, data),\
)
bool sarray_remove(sarray array, void* data);
bool uarray_remove(uarray array, void* data);

// Tries to remove data from this array, using predicate p to check the data
// and returning true if it succeeeds. This will remove the first copy of data
// it finds.
#define array_removep(array, data, p, user) _Generic(array,\
    sarray: sarray_removep(array, data, p, user),\
    uarray: uarray_removep(array, data, p, user),\
)
bool sarray_removep(sarray array, void* data, equality_predicate p, void* user);
bool uarray_removep(uarray array, void* data, equality_predicate p, void* user);

// Removes the element at position in this array.
#define array_remove_at(array, position) _Generic(array,\
    sarray: sarray_remove_at(array, position),\
    uarray: uarray_remove_at(array, position),\
)
void sarray_remove_at(sarray array, uint16 position);
void uarray_remove_at(uarray array, uint16 position);

// Returns the element at position in this array.
#define array_get(array, position) _Generic(array,\
    sarray: sarray_get(array, position),\
    uarray: uarray_get(array, position),\
)
void* sarray_get(sarray array, uint16 position);
void* uarray_get(uarray array, uint16 position);

// Replaces the element at position in this array with data.
#define array_set(array, position, data) _Generic(array,\
    sarray: sarray_set(array, position, data),\
    uarray: uarray_set(array, position, data),\
)
void sarray_set(sarray array, uint16 position, void* data);
void uarray_set(uarray array, uint16 position, void* data);

// Performs a heapsort on array using predicate p for comparison.
#define array_sort(array, p, user) _Generic(array,\
    sarray: sarray_sort(array, p, user),\
    uarray: uarray_sort(array, p, user),\
)
void sarray_sort(sarray array, comparison_predicate p, void* user);
void uarray_sort(uarray array, comparison_predicate p, void* user);

// Calls d on each object in the array. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
#define array_foreach(array, d) _Generic(array,\
    sarray: sarray_foreach(array, d),\
    uarray: uarray_foreach(array, d),\
)
void sarray_foreach(sarray array, foreach_delegate d, void* user);
void uarray_foreach(uarray array, foreach_delegate d, void* user);

#endif // DF_CORE_ARRAY
