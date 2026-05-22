#ifndef BTREE_H
#define BTREE_H

/*
 * INSERTION RULE: Binary Search Tree (BST)
 * - The first element becomes the root.
 * - For each new value: if value < current node, go left; if value > current node, go right.
 * - Insert at the first empty (NULL) child position found.
 * - All values are distinct; duplicate values are rejected on insert.
 */

#define ARRAY_SIZE 68

typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

const int *get_component_array(void);
int get_component_count(void);

TreeNode *create_node(int value);
TreeNode *insert_bst(TreeNode *root, int value);
TreeNode *build_tree_from_array(const int *values, int count);
TreeNode *search_node(const TreeNode *root, int value);
TreeNode *find_parent(const TreeNode *root, int value, const TreeNode **out_parent);

void print_root(const TreeNode *root);
void print_leaf_nodes(const TreeNode *root);
void print_siblings(const TreeNode *root, int value);
void print_parent(const TreeNode *root, int value);
void print_grandchildren(const TreeNode *root, int value);
void free_tree(TreeNode *root);

#endif
