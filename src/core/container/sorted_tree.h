#ifndef DF_SORTED_TREE_H
#define DF_SORTED_TREE_H
#include <inttypes.h>
#include <stdbool.h>

typedef struct sorted_tree
{
    uint32_t     key;
    int8_t       balance_factor;
    int          height;
    void*        data;
    struct sorted_tree* left_child;
    struct sorted_tree* right_child;
}
sorted_tree;

sorted_tree* create_sorted_tree(uint32_t key, void* data);
void* sorted_tree_get(sorted_tree* tree, uint32_t key);
#define sorted_tree_contains(tree, key) ( sorted_tree_get(tree, key) != 0 )
bool sorted_tree_set(sorted_tree* tree, uint32_t key, void* data, bool can_add_new);
void destroy_sorted_tree_full(sorted_tree* tree, bool deep);
#define destroy_sorted_tree(tree) { destroy_sorted_tree_full(tree, false); tree = 0; }
#define destroy_sorted_tree_deep(tree) { destroy_sorted_tree_full(tree, true); tree = 0; }

void print_sorted_tree(sorted_tree* tree);

#endif
