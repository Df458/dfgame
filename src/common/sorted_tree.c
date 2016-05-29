#include <stdlib.h>
#include "array_list.h"
#include "sorted_tree.h"
#include "util.h"

sorted_tree* create_sorted_tree(uint32_t key, void* data)
{
    sorted_tree* tree = salloc(sizeof(sorted_tree));
    tree->key  = key;
    tree->data = data;
    tree->balance_factor = 0;
    tree->height = 0;
    tree->left_child = 0;
    tree->right_child = 0;

    return tree;
}

void* sorted_tree_get(sorted_tree* tree, uint32_t key)
{
    if(tree->key == key)
        return tree->data;
    if(key < tree->key && tree->left_child)
        return sorted_tree_get(tree->left_child, key);
    if(key > tree->key && tree->right_child)
        return sorted_tree_get(tree->right_child, key);

    return 0;
}

void sorted_tree_balance(sorted_tree* tree)
{
    if(tree->balance_factor > -2 && tree->balance_factor < 2)
        return;

    bool left_1 = tree->balance_factor > 0;
    sorted_tree* child = left_1 ? tree->left_child : tree->right_child;
    // If this is true, then we can perform a single rotation. Otherwise, two
    // are necessary.
    bool straight_rotation = left_1 ? child->right_child == 0 : child->left_child == 0;
    if(!straight_rotation) {
        // Rotate once extra to make the final rotation straight
        sorted_tree* second_child = left_1 ? child->right_child : child->left_child;
        if(left_1) {
            tree->left_child = child->right_child;
            child->right_child = 0;
            tree->left_child->left_child = child;
            child = tree->left_child;
        } else {
            tree->right_child = child->left_child;
            child->left_child = 0;
            tree->right_child->right_child = child;
            child = tree->right_child;
        }
    }
    // Rotate the straight segment
    if(left_1) {
        if(child->right_child)
            child->right_child->right_child = tree->right_child;
        else
            child->right_child = tree->right_child;
        tree->right_child = child;
        tree->left_child = child->left_child;
        child->left_child = 0;
    } else {
        if(child->left_child)
            child->left_child->left_child = tree->left_child;
        else
            child->left_child = tree->left_child;
        tree->left_child = child;
        tree->right_child = child->right_child;
        child->right_child = 0;
    }
    void* td    = tree->data;
    int   tk    = tree->key;
    tree->data  = child->data;
    tree->key   = child->key;
    child->data = td;
    child->key  = tk;
}

void update_balance_factors(sorted_tree* tree)
{
    tree->balance_factor = 0;
    int lh = 0;
    int rh = 0;
    if(tree->left_child) {
        update_balance_factors(tree->left_child);
        lh = tree->left_child->height + 1;
    }
    if(tree->right_child) {
        update_balance_factors(tree->right_child);
        rh = tree->right_child->height + 1;
    }

    tree->height = (lh > rh ? lh : rh);
    tree->balance_factor = lh - rh;
    if(abs(tree->balance_factor) > 1) {
        sorted_tree_balance(tree);
        update_balance_factors(tree); // Lord have mercy on my soul
    }
}

bool sorted_tree_set(sorted_tree* tree, uint32_t key, void* data, bool can_add_new)
{
    if(key == tree->key) {
        tree->data = data;
        return true;
    }
    if(key < tree->key) {
        if(tree->left_child) {
            if(sorted_tree_set(tree->left_child, key, data, can_add_new)) {
                int lh = tree->left_child->height;
                int rh = tree->right_child ? tree->right_child->height : 0;
                tree->height = (lh > rh ? lh : rh) + 1;
                tree->balance_factor = lh - rh;
                if(abs(tree->balance_factor) > 1) {
                    sorted_tree_balance(tree);
                    update_balance_factors(tree);
                }

                /* if(b != tree->left_child->height) { */
                /*     tree->balance_factor += 1; */
                /*     info("Rebalancing %d: %d", tree->key, tree->balance_factor); */
                /*     sorted_tree_balance(tree); */
                /*     update_balance_factors(tree); */
                /* } */
                return true;
            } else
                return false;
        } else if(!can_add_new) {
            return false;
        } else {
            tree->left_child = create_sorted_tree(key, data);
            int lh = tree->left_child->height;
            int rh = tree->right_child ? tree->right_child->height : 0;
            tree->height = (lh > rh ? lh : rh) + 1;
            tree->balance_factor = lh - rh;
            return true;
        }
    } else {
        if(tree->right_child) {
            if(sorted_tree_set(tree->right_child, key, data, can_add_new)) {
                int lh = tree->left_child ? tree->left_child->height : 0;
                int rh = tree->right_child->height;
                tree->height = (lh > rh ? lh : rh) + 1;
                tree->balance_factor = lh - rh;
                if(abs(tree->balance_factor) > 1) {
                    sorted_tree_balance(tree);
                    update_balance_factors(tree);
                }

                /* if(b != tree->left_child->height) { */
                /*     tree->balance_factor += 1; */
                /*     info("Rebalancing %d: %d", tree->key, tree->balance_factor); */
                /*     sorted_tree_balance(tree); */
                /*     update_balance_factors(tree); */
                /* } */
                return true;
            } else
                return false;
        } else if(!can_add_new) {
            return false;
        } else {
            tree->right_child = create_sorted_tree(key, data);
            int lh = tree->left_child ? tree->left_child->height : 0;
            int rh = tree->right_child->height;
            tree->height = (lh > rh ? lh : rh) + 1;
            tree->balance_factor = lh - rh;
            return true;
        }
    }
}

void destroy_sorted_tree_full(sorted_tree* tree, bool deep)
{
    if(tree->left_child)
        destroy_sorted_tree_full(tree->left_child, deep);
    if(tree->right_child)
        destroy_sorted_tree_full(tree->right_child, deep);
    if(deep)
        sfree(tree->data);
    sfree(tree);
}

void print_sorted_tree(sorted_tree* tree)
{
    array_list* child_list = create_array_list();
    array_list* child_list_2 = create_array_list();
    array_list_push(child_list, tree);
    int j = abs(tree->height);
    while(child_list->length > 0) {
        for(int i = 0; i < j; ++i) {
            fprintf(stderr, "    ");
        }
        --j;
        bool c = false;
        while(child_list->length > 0) {
            sorted_tree* temp = array_list_pop(child_list);
            if(temp == 0) {
                fprintf(stderr, "    ");
                array_list_push(child_list_2, 0);
                array_list_push(child_list_2, 0);
                continue;
            }
            c = true;
            fprintf(stderr, "<%02d,%02d> ", temp->key, temp->balance_factor);
            if(temp->left_child)
                array_list_push(child_list_2, temp->left_child);
            else
                array_list_push(child_list_2, 0);

            if(temp->right_child)
                array_list_push(child_list_2, temp->right_child);
            else
                array_list_push(child_list_2, 0);
        }
        /* for(int i = 0; i < child_list_2->length; ++i) { */
        /*     array_list_push(child_list, array_list_get(child_list_2, i)); */
        /* } */
        while(child_list_2->length > 0) {
            /* array_list_pop(child_list_2); */
            array_list_push(child_list, array_list_pop(child_list_2));
        }
        fprintf(stderr, "\n");
        if(!c)
            break;
    }
}
