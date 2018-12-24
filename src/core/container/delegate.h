#ifndef DF_CORE_DELEGATE
#define DF_CORE_DELEGATE
#include "core/types.h"

// Used to compare elements in a container, returning if the elements are the
// same.
delegate(bool, equality_predicate, void* o1, void* o2, void* user);

// Used to compare elements in a container, returning if the first is greater,
// less than, or equal to the second.
delegate(comparison, comparison_predicate, void* o1, void* o2, void* user);

// These are used by the foreach_delegate to communicate with the container
// that it's iterating
typedef enum iter_decision {
    DECISION_CONTINUE = 0,
    DECISION_DELETE,
    DECISION_REPLACE,
    DECISION_BREAK,
    DECISION_BREAK_DELETE,
    DECISION_BREAK_REPLACE,
} iter_decision;
typedef struct iter_result {
    iter_decision decision;
    void* replacement_value;
} iter_result;

// Helper macros for quick decisions
#define iter_continue (iter_result) { .decision = DECISION_CONTINUE }
#define iter_break (iter_result) { .decision = DECISION_BREAK }
#define iter_delete (iter_result) { .decision = DECISION_DELETE }
#define iter_break_delete (iter_result) { .decision = DECISION_BREAK_DELETE }
#define iter_replace(data) (iter_result) { .decision = DECISION_REPLACE, .replacement_value = data }
#define iter_break_replace(data) (iter_result) { .decision = DECISION_BREAK_REPLACE, .replacement_value = data }

// Used to iterate a container, performing actions on each.
delegate(iter_result, foreach_delegate, void* iter_data, void* user);

#endif // DF_CORE_DELEGATE
