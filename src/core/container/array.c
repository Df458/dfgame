// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "array.h"

#include "alloc.h"
#include "log.h"
#include "power.h"
#include <assert.h>

// Struct Definitions

// Represents an order-preserving auto-resizing array. The order of the contents
// can only be changed manually.
typedef struct sarray {
    uint16 size;
    uint16 alloc_size;
    void** data;
}* sarray;

// Represents an unsorted auto-resizing array. The order of the contents is
// subject to change, but this structure is more speed efficient than sarray.
typedef struct uarray {
    uint16 size;
    uint16 alloc_size;
    void** data;
}* uarray;

// ----------------------------------------------------------------------------

// Private Functions

// Shifts the elements of an array from start to end, leaving a vacant space at
// start and filling (removing) the contents at end. The shift direction is
// determined based on the relative positions of the points. If the points are
// the same, this function does nothing.
//
// NOTE: The points are NOT bounds-checked. Inputs are assumed to be correct,
//       and must be validated by the calling function.
static void shift(void** data, uint16 start, uint16 end) {
    // Determine the direction that we'll be shifting.
    int8 shiftval = 1;
    if(end > start)
        shiftval = -1;

    for(uint16 i = end; i != start; i += shiftval) {
        data[i] = data[i + shiftval];
    }
}

// Checks if the array's new size exceeds the allocated length, and grows the
// data to the next power of two spaces. Obviously, the size should be updated
// before this function is called.
static void sarray_resize_if_full(sarray array) {
    if(array->size > array->alloc_size)
    {
        if(array->alloc_size == 0)
            array->alloc_size = 1;
        else
            array->alloc_size = next_power_of_two(array->alloc_size);

        array->data = resalloc(array->data, array->alloc_size * sizeof(void*));
    }
}
static void uarray_resize_if_full(uarray array) {
    if(array->size > array->alloc_size)
    {
        if(array->alloc_size == 0)
            array->alloc_size = 1;
        else
            array->alloc_size = next_power_of_two(array->alloc_size);

        array->data = resalloc(array->data, array->alloc_size * sizeof(void*));
    }
}

// Heap-sorts an array, using predicate p to determine ordering.
static void heap_sort(void** data, uint16 size, comparison_predicate p) {
    uint16 i, index, back_index;
    void* temp;
    // Build the heap
    for(i = 0; i < size; ++i) {
        index = i;
        back_index = (index - 1) / 2;
        temp = data[index];

        while(index > 0 && p(temp, data[back_index]) == COMPARE_GREATER_THAN) {
            data[index] = data[back_index];
            index = back_index;
            back_index = (index - 1) / 2;
        }
        data[index] = temp;
    }

    for(i = size - 1; i > 0; --i) {
        temp = data[0];
        data[0] = data[i];
        data[i] = temp;

        // Shift
        back_index = 0;
        index = 1;
        temp = data[0];
        while(index <= i - 1) {
            if(index != (i - 1) && p(data[index], data[index + 1]) == COMPARE_LESS_THAN)
                ++index;
            if(p(temp, data[index]) == COMPARE_LESS_THAN) {
                data[back_index] = data[index];
                back_index = index;
                index = 2 * back_index + 1;
            } else {
                break;
            }
        }
        data[back_index] = temp;
    }
}

// ----------------------------------------------------------------------------

// Creates a new array with enough space allocated to hold up to size members.
// 0 is a valid size, since it will grow to fit new members.
sarray sarray_new(uint16 size) {
    sarray new_array = salloc(sizeof(sarray));

    new_array->data       = NULL;
    new_array->size       = 0;
    new_array->alloc_size = size;

    if(size > 0)
        new_array->data = scalloc(size, sizeof(void*));

    return new_array;
}
uarray uarray_new(uint16 size) {
    uarray new_array = salloc(sizeof(uarray));

    new_array->data       = NULL;
    new_array->size       = 0;
    new_array->alloc_size = size;

    if(size > 0)
        new_array->data = scalloc(size, sizeof(void*));

    return new_array;
}

// Frees the array. NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _sarray_free(sarray array) {
    if(array && array->data != NULL)
        sfree(array->data);
    sfree(array);
}
void _uarray_free(uarray array) {
    if(array && array->data != NULL)
        sfree(array->data);
    sfree(array);
}

// Returns the number of actual members stored in this array.
uint16 sarray_size(sarray array) {
    return array->size;
}
uint16 uarray_size(uarray array) {
    return array->size;
}

// Adds a new member to the end of this array, resizing it if necessary.
void sarray_add(sarray array, void* data) {
    array->size++;
    sarray_resize_if_full(array);

    array->data[array->size - 1] = data;
}
void uarray_add(uarray array, void* data) {
    array->size++;
    uarray_resize_if_full(array);

    array->data[array->size - 1] = data;
}

// Adds a new member to a specified position in this array, resizing it if
// necessary.
void sarray_insert(sarray array, void* data, uint16 position) {
    if(position == array->size) {
        sarray_add(array, data);
        return;
    }

    assert(position < array->size);
    array->size++;
    sarray_resize_if_full(array);

    shift(array->data, position, array->size);
    array->data[position] = data;
}
void uarray_insert(uarray array, void* data, uint16 position) {
    if(position == array->size) {
        uarray_add(array, data);
        return;
    }

    assert(position < array->size);
    array->size++;
    uarray_resize_if_full(array);

    uarray_add(array, array->data[position]);
    array->data[position] = data;
}

// Returns true if data is a member of this array, or false if it isn't.
bool sarray_contains(sarray array, void* data) {
    return sarray_find(array, data) != -1;
}
bool uarray_contains(uarray array, void* data) {
    return uarray_find(array, data) != -1;
}

// Returns true if data is a member of this array, or false if it isn't. Uses
// predicate p to check the array for data.
bool sarray_containsp(sarray array, void* data, equality_predicate p) {
    return sarray_findp(array, data, p) != -1;
}
bool uarray_containsp(uarray array, void* data, equality_predicate p) {
    return uarray_findp(array, data, p) != -1;
}

// Returns the position of data in this array, or INVALID_INDEX if array does
// not contain data.
int32 sarray_find(sarray array, void* data) {
    for(int32 i = 0; i < array->size; ++i)
        if(array->data[i] == data)
            return i;
    return INVALID_INDEX;
}
int32 uarray_find(uarray array, void* data) {
    for(int32 i = 0; i < array->size; ++i)
        if(array->data[i] == data)
            return i;
    return INVALID_INDEX;
}

// Returns the position of data using predicate p to check the array, or
// INVALID_INDEX if array does not contain data.
int32 sarray_findp(sarray array, void* data, equality_predicate p) {
    for(int32 i = 0; i < array->size; ++i)
        if(p(array->data[i], data))
            return i;
    return INVALID_INDEX;
}
int32 uarray_findp(uarray array, void* data, equality_predicate p) {
    for(int32 i = 0; i < array->size; ++i)
        if(p(array->data[i], data))
            return i;
    return INVALID_INDEX;
}

// Tries to remove data from this array, returning true if it succeeeds. This
// will remove the first copy of data it finds.
bool sarray_remove(sarray array, void* data) {
    int32 index = sarray_find(array, data);

    if(index == INVALID_INDEX) {
        warn("Tried to remove object 0x%x from an sarray, but it couldn't be found", data);
        return false;
    }
    sarray_remove_at(array, index);

    return true;
}
bool uarray_remove(uarray array, void* data) {
    int32 index = uarray_find(array, data);

    if(index == INVALID_INDEX) {
        warn("Tried to remove object 0x%x from a uarray, but it couldn't be found", data);
        return false;
    }
    uarray_remove_at(array, index);

    return true;
}

// Tries to remove data from this array, using predicate p to check the data
// and returning true if it succeeeds. This will remove the first copy of data
// it finds.
bool sarray_removep(sarray array, void* data, equality_predicate p) {
    int32 index = sarray_findp(array, data, p);

    if(index == INVALID_INDEX) {
        warn("Tried to remove object 0x%x from an sarray, but it couldn't be found", data);
        return false;
    }
    sarray_remove_at(array, index);

    return true;
}
bool uarray_removep(uarray array, void* data, equality_predicate p) {
    int32 index = uarray_findp(array, data, p);

    if(index == INVALID_INDEX) {
        warn("Tried to remove object 0x%x from a uarray, but it couldn't be found", data);
        return false;
    }
    uarray_remove_at(array, index);

    return true;
}

// Removes the element at position in this array.
void sarray_remove_at(sarray array, uint16 position) {
    assert(position < array->size);
    --array->size;
    shift(array->data, array->size, position);
}
void uarray_remove_at(uarray array, uint16 position) {
    assert(position < array->size);
    --array->size;
    array->data[position] = array->data[array->size];
}

// Returns the element at position in this array.
void* sarray_get(sarray array, uint16 position) {
    assert(position < array->size);

    return array->data[position];
}
void* uarray_get(uarray array, uint16 position) {
    assert(position < array->size);

    return array->data[position];
}

// Replaces the element at position in this array with data.
void sarray_set(sarray array, uint16 position, void* data) {
    assert(position < array->size);

    array->data[position] = data;
}
void uarray_set(uarray array, uint16 position, void* data) {
    assert(position < array->size);

    array->data[position] = data;
}

// Performs a heapsort on array using predicate p for comparison.
void sarray_sort(sarray array, comparison_predicate p) {
    heap_sort(array->data, array->size, p);
}
void uarray_sort(uarray array, comparison_predicate p) {
    heap_sort(array->data, array->size, p);
}

#undef LOG_CATEGORY