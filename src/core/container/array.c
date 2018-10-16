// Log category, used to filter logs
#define LOG_CATEGORY "Core"

#include "array.h"

#include "alloc.h"
#include "check.h"
#include "power.h"
#include <assert.h>

// Private Functions

// Shifts the elements of an array from start to end, leaving a vacant space at
// start and filling (removing) the contents at end. The shift direction is
// determined based on the relative positions of the points. If the points are
// the same, this function does nothing.
//
// NOTE: The points are NOT bounds-checked. Inputs are assumed to be correct,
//       and must be validated by the calling function.
static void shift(void* data, uint16 start, uint16 end, uint16 member_size) {
    // Determine the direction that we'll be shifting.
    int8 shiftval = 1;
    if(end > start) {
        shiftval = -1;
    }

    for(uint16 i = end; i != start; i += shiftval) {
        memcpy(data + (member_size * i), data + (member_size * (i + shiftval)), member_size);
    }
}

// Checks if the array's new size exceeds the allocated length, and grows the
// data to the next power of two spaces. Obviously, the size should be updated
// before this function is called.
static void array_resize_if_full(array a) {
    if(a->length > a->alloc_length) {
        if(a->alloc_length == 0) {
            a->alloc_length = 1;
        } else {
            a->alloc_length = next_power_of_two(a->alloc_length);
        }

        a->data = resalloc(a->data, a->alloc_length * a->member_size);
    }
}

// Heap-sorts an array, using predicate p to determine ordering.
static void heap_sort(void* data, uint16 length, uint16 member_size, comparison_predicate p, void* user) {
    uint16 i, index, back_index;
    void* temp = salloc(member_size);
    // Build the heap

    for(i = 0; i < length; ++i) {
        index = i;
        back_index = (index - 1) / 2;
        memcpy(temp, data + (member_size * index), member_size);

        while(index > 0 && p(temp, data + (member_size * back_index), user) == COMPARE_GREATER_THAN) {
            memcpy(data + (member_size * index), data + (member_size * back_index), member_size);
            index = back_index;
            back_index = (index - 1) / 2;
        }
        memcpy(data + (member_size * index), temp, member_size);
    }

    for(i = length - 1; i > 0; --i) {
        memcpy(temp, data, member_size);
        memcpy(data, data + (member_size * i), member_size);
        memcpy(data + (member_size * i), temp, member_size);

        // Shift
        back_index = 0;
        index = 1;
        memcpy(temp, data, member_size);
        while(index <= i - 1) {
            if(index != (i - 1) && p(data + (member_size * index), data + (member_size * (index + 1)), user) == COMPARE_LESS_THAN) {
                ++index;
            }
            if(p(temp, data + (member_size * index), user) == COMPARE_LESS_THAN) {
                memcpy(data + (member_size * back_index), data + (member_size * index), member_size);
                back_index = index;
                index = 2 * back_index + 1;
            } else {
                break;
            }
        }
        memcpy(data + (member_size * back_index), temp, member_size);
    }

    sfree(temp);
}

// ----------------------------------------------------------------------------

// Creates a new array with enough space allocated to hold up to size members.
// 0 is a valid size, since it will grow to fit new members.
array array_new_common(uint16 size, uint16 reserve) {
    check_return(reserve > 0, "Array elements must have a size", NULL);

    array new_array = salloc(sizeof(struct array));

    new_array->member_size  = size;
    new_array->length       = 0;
    new_array->alloc_length = reserve;

    new_array->data         = NULL;

    if(reserve > 0) {
        new_array->data = scalloc(new_array->alloc_length, new_array->member_size);
    }

    return new_array;
}
array array_new(uint16 size, uint16 reserve) {
    array new_array = array_new_common(size, reserve);

    if(new_array) {
        new_array->preserve_order = false;
    }

    return new_array;
}
array array_new_ordered(uint16 size, uint16 reserve) {
    array new_array = array_new_common(size, reserve);

    if(new_array) {
        new_array->preserve_order = true;
    }

    return new_array;
}

// Frees the array. NOTE: Don't call these functions. Use the macros without
// the leading _ instead, as they also NULL your pointer.
void _array_free(array a) {
    check_return(a, "Array is NULL", );

    if(a->data != NULL) {
        sfree(a->data);
    }
    sfree(a);
}

// Returns the number of actual members stored in this array.
uint16 array_get_length(array a) {
    check_return(a, "Array is NULL", 0);

    return a->length;
}

// Adds a new member to the end of this array, resizing it if necessary.
void _array_add(array a, void* data, uint16 size) {
    check_return(a, "Array is NULL", );
    check_return(a->member_size == size, "Array member size mismatch (%d != %d)", , a->member_size, size);

    array_insert(a, data, a->length);
}

// Adds a new member to a specified position in this array, resizing it if necessary.
void array_insert(array a, void* data, uint16 position) {
    check_return(a, "Array is NULL", );

    assert(position <= a->length);
    a->length++;
    array_resize_if_full(a);

    if(position + 1 < a->length) {
        if(a->preserve_order) { // Shift the contents forward if we care about order
            shift(a->data, position, a->length, a->member_size);
        } else { // Or just copy the element into the last slot instead
            memcpy(a->data + (a->member_size * (a->length - 1)), a->data + (a->member_size * position), a->member_size);
        }
    }

    memcpy(a->data + (a->member_size * position), data, a->member_size);
}

// Returns true if data is a member of this array, or false if it isn't.
bool array_contains(array a, void* data) {
    check_return(a, "Array is NULL", false);

    return array_find(a, data) != ARRAY_INDEX_INVALID;
}

// Returns true if data is a member of this array, or false if it isn't. Uses
// predicate p to check the array for data.
bool array_containsp(array a, void* data, equality_predicate p, void* user) {
    check_return(a, "Array is NULL", false);

    return array_findp(a, data, p, user) != ARRAY_INDEX_INVALID;
}

// Returns the position of data in this array, or ARRAY_INDEX_INVALID if array does
// not contain data.
int32 array_find(array a, void* data) {
    check_return(a, "Array is NULL", ARRAY_INDEX_INVALID);

    for(int32 i = 0; i < a->length; ++i) {
        if(!memcmp(a->data + (a->member_size * i), data, a->member_size)) {
            return i;
        }
    }
    return ARRAY_INDEX_INVALID;
}

// Returns the position of data using predicate p to check the array, or
// ARRAY_INDEX_INVALID if array does not contain data.
int32 array_findp(array a, void* data, equality_predicate p, void* user) {
    check_return(a, "Array is NULL", ARRAY_INDEX_INVALID);

    for(int32 i = 0; i < a->length; ++i) {
        if(p(a->data + (a->member_size * i), data, user)) {
            return i;
        }
    }
    return ARRAY_INDEX_INVALID;
}

// Tries to remove data from this array, returning true if it succeeeds. This
// will remove the first copy of data it finds.
bool array_remove(array a, void* data) {
    check_return(a, "Array is NULL", false);

    int32 index = array_find(a, data);
    check_return(index != ARRAY_INDEX_INVALID, "Tried to remove object 0x%x from an array, but it couldn't be found", false, data);
    array_remove_at(a, index);

    return true;
}

// Tries to remove data from this array, using predicate p to check the data
// and returning true if it succeeeds. This will remove the first copy of data
// it finds.
bool array_removep(array a, void* data, equality_predicate p, void* user) {
    check_return(a, "Array is NULL", false);

    int32 index = array_findp(a, data, p, user);
    check_return(index != ARRAY_INDEX_INVALID, "Tried to remove object 0x%x from an array, but it couldn't be found", false, data);
    array_remove_at(a, index);

    return true;
}

// Removes the element at the end of this array and returns it
void* array_pop(array a) {
    check_return(a, "Array is NULL", NULL);
    check_return(a->length > 0, "Array is empty", NULL);

    void* data = array_get(a, a->length - 1);
    array_remove_at(a, a->length - 1);

    return data;
}

// Removes the element at position in this array.
void array_remove_at(array a, uint16 position) {
    check_return(a, "Array is NULL", );
    check_return(position < a->length, "Trying to remove out-of-bounds element %d from an array of length %d", , position, a->length);
    --a->length;

    if(position != a->length) {
        if(a->preserve_order) {
            shift(a->data, a->length, position, a->member_size);
        } else {
            memcpy(a->data + (a->member_size * position), a->data + (a->member_size * a->length), a->member_size);
        }
    }
}

// Removes the element pointed to by it from this array
void array_remove_iter(array a, array_iter* it) {
    check_return(a, "Array is NULL", );
    check_return(it && it->is_valid, "Attempting to access an array with an invalid iterator", );

    array_remove_at(a, it->index);
    if(it->increment > 0) {
        it->index -= it->increment;
    }
}

// Returns the element at position in this array.
void* array_get(array a, uint16 position) {
    check_return(a, "Array is NULL", NULL);
    check_return(position < a->length, "Array index %u is out of bounds in array of size %u", NULL, position, a->length);

    return a->data + (a->member_size * position);
}

// Returns the element matching predicate p in this array
void* array_getp(array a, void* data, equality_predicate p, void* user) {
    check_return(a, "Array is NULL", NULL);

    uint16 index = array_findp(a, data, p, user);
    if(index == ARRAY_INDEX_INVALID) {
        return NULL;
    }

    return array_get(a, index);
}

// Replaces the element at position in this array with data.
void array_set(array a, uint16 position, void* data) {
    check_return(a, "Array is NULL", );
    check_return(position < a->length, "Array index %u is out of bounds in array of size %u", , position, a->length);

    memcpy(a->data + (a->member_size * position), data, a->member_size);
}

// Performs a heapsort on array using predicate p for comparison.
void array_sort(array a, comparison_predicate p, void* user) {
    heap_sort(a->data, a->length, a->member_size, p, user);
}

// Calls d on each object in the array. Values an be replaced
// by returning a decision of *_REPLACE, and they can be deleted by returning
// a decision of *_DELETE.
// Values can be safely deleted without worrying about skipping entries, but
// users must still free memory as usual.
void array_foreachd(array a, foreach_delegate d, void* user) {
    // Call d on each item
    for(uint16 i = 0; i < a->length; ++i) {
        iter_result res = d(a->data + (a->member_size * i), user);

        // Respond to delete/replace decisions
        if(res.decision % 3 == 1) { // DELETE or BREAK_DELETE
            array_remove_at(a, i);
            --i;
        } else if(res.decision % 3 == 2) { // REPLACE or BREAK_REPLACE
            array_set(a, i, res.replacement_value);
        }

        // If the decision involves breaking, break
        if(res.decision >= DECISION_BREAK) {
            break;
        }
    }
}

// Gets an iterator to the start of the array
array_iter array_get_start(array a) {
    if(a->length == 0) {
        return (array_iter) {0};
    }

    return (array_iter) {
        .index = 0,
        .increment = 1,
        .data = a->data,
        .is_valid = true
    };
}

// Gets an iterator to the end of the array
array_iter array_get_end(array a) {
    if(a->length == 0) {
        return (array_iter) {0};
    }

    return (array_iter) {
        .index = a->length - 1,
        .increment = -1,
        .data = a->data + (a->member_size * (a->length - 1)),
        .is_valid = true
    };
}

// Advances an iterator to the next spot in the array
void array_get_next(array a, array_iter* i) {
    check_return(i && i->is_valid, "Attempting to advance an invalid iterator", );

    i->index += i->increment;
    if(i->index < a->length) {
        i->data = a->data + (a->member_size * i->index);
    } else {
        i->data = NULL;
        i->is_valid = false;
    }
}

// Advances an iterator to the previous spot in the array
void array_get_prev(array a, array_iter* i) {
    check_return(i && i->is_valid, "Attempting to advance an invalid iterator", );

    i->index -= i->increment;
    if(i->index < a->length) {
        i->data = a->data + (a->member_size * i->index);
    } else {
        i->data = NULL;
        i->is_valid = false;
    }
}

#undef LOG_CATEGORY
