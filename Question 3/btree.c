#include "btree.h"

#include <stdio.h>
#include <stdlib.h>

/* 68 distinct IDs; insertion order shuffled for a non-degenerate BST shape */
static const int COMPONENT_IDS[ARRAY_SIZE] = {
    358, 309, 23, 115, 45, 236, 120, 303, 302, 175, 194, 151, 173, 391, 434, 446,
    41, 82, 195, 360, 16, 58, 280, 414, 112, 177, 136, 378, 72, 333, 126, 288,
    347, 111, 13, 113, 14, 259, 53, 17, 425, 141, 215, 322, 473, 380, 50, 283,
    143, 367, 389, 217, 457, 310, 64, 80, 230, 491, 499, 374, 413, 4, 275, 48,
    415, 328, 102, 184
};

const int *get_component_array(void) {
    return COMPONENT_IDS;
}

int get_component_count(void) {
    return ARRAY_SIZE;
}

TreeNode *create_node(int value) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode *insert_bst(TreeNode *root, int value) {
    if (root == NULL) {
        return create_node(value);
    }

    if (value < root->value) {
        root->left = insert_bst(root->left, value);
    } else if (value > root->value) {
        root->right = insert_bst(root->right, value);
    }

    return root;
}

TreeNode *build_tree_from_array(const int *values, int count) {
    TreeNode *root = NULL;
    int i;

    for (i = 0; i < count; i++) {
        root = insert_bst(root, values[i]);
        if (root == NULL) {
            return NULL;
        }
    }

    return root;
}

TreeNode *search_node(const TreeNode *root, int value) {
    if (root == NULL) {
        return NULL;
    }

    if (value == root->value) {
        return (TreeNode *)root;
    }
    if (value < root->value) {
        return search_node(root->left, value);
    }
    return search_node(root->right, value);
}

TreeNode *find_parent(const TreeNode *root, int value, const TreeNode **out_parent) {
    if (out_parent != NULL) {
        *out_parent = NULL;
    }

    if (root == NULL || root->value == value) {
        return NULL;
    }

    if (root->left != NULL && root->left->value == value) {
        if (out_parent != NULL) {
            *out_parent = root;
        }
        return (TreeNode *)root->left;
    }
    if (root->right != NULL && root->right->value == value) {
        if (out_parent != NULL) {
            *out_parent = root;
        }
        return (TreeNode *)root->right;
    }

    if (value < root->value) {
        return find_parent(root->left, value, out_parent);
    }
    return find_parent(root->right, value, out_parent);
}

void print_root(const TreeNode *root) {
    if (root == NULL) {
        printf("Tree is empty. No root node.\n");
        return;
    }
    printf("Root node: %d\n", root->value);
}

static void collect_leaves(const TreeNode *node) {
    if (node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        printf("  %d\n", node->value);
        return;
    }

    collect_leaves(node->left);
    collect_leaves(node->right);
}

void print_leaf_nodes(const TreeNode *root) {
    if (root == NULL) {
        printf("Tree is empty. No leaf nodes.\n");
        return;
    }

    printf("Leaf nodes:\n");
    collect_leaves(root);
}

void print_siblings(const TreeNode *root, int value) {
    const TreeNode *parent = NULL;
    int found_sibling = 0;

    if (search_node(root, value) == NULL) {
        return;
    }

    find_parent(root, value, &parent);

    if (parent == NULL) {
        printf("Node %d is the root. It has no siblings.\n", value);
        return;
    }

    printf("Siblings of node %d:\n", value);
    if (parent->left != NULL && parent->left->value != value) {
        printf("  %d\n", parent->left->value);
        found_sibling = 1;
    }
    if (parent->right != NULL && parent->right->value != value) {
        printf("  %d\n", parent->right->value);
        found_sibling = 1;
    }

    if (!found_sibling) {
        printf("  (none)\n");
    }
}

void print_parent(const TreeNode *root, int value) {
    const TreeNode *parent = NULL;

    if (search_node(root, value) == NULL) {
        return;
    }

    find_parent(root, value, &parent);

    if (parent == NULL) {
        printf("Node %d is the root. It has no parent.\n", value);
        return;
    }

    printf("Parent of node %d: %d\n", value, parent->value);
}

void print_grandchildren(const TreeNode *root, int value) {
    const TreeNode *node = search_node(root, value);
    int count = 0;

    if (node == NULL) {
        return;
    }

    printf("Grandchildren of node %d:\n", value);

    if (node->left != NULL) {
        if (node->left->left != NULL) {
            printf("  %d\n", node->left->left->value);
            count++;
        }
        if (node->left->right != NULL) {
            printf("  %d\n", node->left->right->value);
            count++;
        }
    }

    if (node->right != NULL) {
        if (node->right->left != NULL) {
            printf("  %d\n", node->right->left->value);
            count++;
        }
        if (node->right->right != NULL) {
            printf("  %d\n", node->right->right->value);
            count++;
        }
    }

    if (count == 0) {
        printf("  (none)\n");
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
