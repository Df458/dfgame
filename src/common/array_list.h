#ifndef DF_ARRAY_LIST_H
#define DF_ARRAY_LIST_H

#include <inttypes.h>

typedef uint16_t array_list_size_t;

typedef struct array_list
{
    void**            data;
    array_list_size_t data_size;
    array_list_size_t length;
}
array_list;

array_list* create_array_list_size(array_list_size_t size);
#define create_array_list() create_array_list_size(0)
void array_list_destroy_full(array_list* list, uint8_t deep);
#define array_list_destroy(list) { array_list_destroy_full(list, 0); list = 0; }
#define array_list_destroy_deep(list) { array_list_destroy_full(list, 1); list = 0; }

int array_list_add(array_list* list, int index, void* ptr);
void* array_list_get(array_list* list, int index);
#define array_list_push(list, ptr) array_list_add(list, -1, ptr)
void* array_list_remove_at(array_list* list, int index);
#define array_list_pop(list) array_list_remove_at(list, -1)
int array_list_remove(array_list* list, void* data);
int array_list_index(array_list* list, void* data);


#endif
