#ifndef DF_CORE_PREDICATE
#define DF_CORE_PREDICATE
#include "types.h"

// Used to compare elements in a container, returning if the elements are the
// same.
typedef bool (*equality_predicate)(void* o1, void* o2);

// Used to compare elements in a container, returning if the first is greater,
// less than, or equal to the second.
typedef comparison (*comparison_predicate)(void* o1, void* o2);

#endif // DF_CORE_PREDICATE
