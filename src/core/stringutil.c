#define LOG_CATEGORY "Core"
#include "stringutil.h"

#include "core/check.h"
#include "core/container/array.h"
#include "core/memory/alloc.h"

#include <stdarg.h>
#include <stdio.h>

char* saprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* final = vsaprintf(format, args);
	va_end(args);

    return final;
}
char* vsaprintf(const char* format, va_list args) {
    if(!format) {
        return NULL;
    }

    va_list tempargs;
    size_t length;
	va_copy(tempargs, args);

    length = vsnprintf(0, 0, format, tempargs);
    va_end(tempargs);

	++length;
    char* final = mscalloc(length, char);

    vsnprintf(final, length, format, args);

    return final;
}
int nstrcmp(const char* str_a, const char* str_b) {
    if(str_a == NULL) {
        if(str_b == NULL) {
            return COMPARE_EQUAL_TO;
        }

        return COMPARE_LESS_THAN;
    } else if(str_b == NULL) {
        return COMPARE_GREATER_THAN;
    }

    return strcmp(str_a, str_b);
}
int nstrncmp(const char* str_a, const char* str_b, size_t len) {
    if(str_a == NULL) {
        if(str_b == NULL) {
            return COMPARE_EQUAL_TO;
        }

        return COMPARE_LESS_THAN;
    } else if(str_b == NULL) {
        return COMPARE_GREATER_THAN;
    }

    return strncmp(str_a, str_b, len);
}
char** nstrsplit(const char* str_a, const char* str_b, uint16* count) {
    check_return(count != NULL, "count is null", NULL);

    // Set count early, just in case of invalid args
    *count = 0;

    check_return(!nstrempty(str_a), "str_a is null or empty", NULL);
    check_return(!nstrempty(str_b), "str_b is null or empty", NULL);

    char* tmp_origin = nstrdup(str_a);
    char* tmp = tmp_origin;
    char* ptr = tmp;
    array str_array = array_mnew_ordered(char*, 4);

    while((ptr = strsep(&tmp, str_b)) != NULL) {
        if(!nstrempty(ptr)) {
            array_add(str_array, ptr);
        }
    }

    char** output = NULL;
    *count = array_get_length(str_array);
    if(count > 0) {
        output = mscalloc(*count, char*);
        array_foreach(str_array, it) {
            output[it.index] = nstrdup(array_iter_data(it, char*));
        }
    }

    array_free(str_array);
    sfree(tmp_origin);

    return output;
}
bool aisi(const char* str) {
    check_return(str, "str is NULL", false);
    for(int i = 0; str[i] != '\0'; ++i) {
        if((str[i] < '0' || str[i] > '9') && str[i] != '-') {
            return false;
        }
    }

    return true;
}
bool aisf(const char* str) {
    check_return(str, "str is NULL", false);
    for(int i = 0; str[i] != '\0'; ++i) {
        if((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-') {
            return false;
        }
    }

    return true;
}
