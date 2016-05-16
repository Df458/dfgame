#include "util.h"
#include "array_list.h"
#include <stdlib.h>

array_list* create_array_list_size(array_list_size_t size)
{
    array_list* list = salloc(sizeof(array_list));
    if(size <= 0)
        size = 1;
    list->data = scalloc(size, sizeof(void*));
    list->length = 0;
    list->data_size = size;

    return list;
}

void array_list_destroy_full(array_list* list, uint8_t deep)
{
    uint16_t i;
    if(deep) {
        for(i = 0; i < list->length; ++i)
            sfree(list->data[i]);
    }
    sfree(list->data);
    sfree(list);
}

int array_list_add(array_list* list, int index, void* ptr)
{
    if(index < 0) {
        index = list->length;
    }
    if(index > list->data_size || list->length == list->data_size) {
        list->data_size *= 2;
        list->data = resalloc(list->data, list->data_size * sizeof(void*));
    }
    if(index < list->length) {
        int i;
        for(i = list->length; i > index; --i)
            list->data[i] = list->data[i - 1];
    }
    list->data[index] = ptr;
    list->length++;

    return list->length;
}

void* array_list_get(array_list* list, int index)
{
    if(index == -1)
        index = list->length - 1;

    if(index > list->length || index < 0)
        return 0;
    return list->data[index];
}

void* array_list_remove_at(array_list* list, int index)
{
    if(index == -1)
        index = list->length - 1;

    if(index > list->length || index < 0)
        return 0;
    void* ret = list->data[index];

    for(int i = index; i < list->length - 1; ++i)
        list->data[i] = list->data[i + 1];
    list->data[list->length - 1] = 0;
    list->length--;

    return ret;
}

int array_list_remove(array_list* list, void* data)
{
    int index = array_list_index(list, data);
    if(index != -1) {
        array_list_remove_at(list, index);
        return index;
    }
    return -1;
}

int array_list_index(array_list* list, void* data)
{
    for(int i = 0; i < list->length; ++i) {
        if(list->data[i] == data)
            return i;
    }

    return -1;
}
