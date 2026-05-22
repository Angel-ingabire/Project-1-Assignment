#include "btree.h"

#include <stdio.h>

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void print_insertion_rule(void) {
    printf("\n=== Binary Tree Insertion Rule ===\n");
    printf("Structure: Binary Search Tree (BST)\n");
    printf("- First value becomes the root.\n");
    printf("- Smaller values are inserted in the left subtree.\n");
    printf("- Larger values are inserted in the right subtree.\n");
    printf("- Insertion stops at the first NULL child position.\n");
    printf("- All 68 component IDs are distinct.\n");
    printf("==================================\n\n");
}

static void print_menu(void) {
    printf("\n========== Binary Tree Menu ==========\n");
    printf("1. Print root node\n");
    printf("2. Print all leaf nodes\n");
    printf("3. Query a node (siblings, parent, grandchildren)\n");
    printf("4. Insert a new node\n");
    printf("5. Show insertion time complexity analysis\n");
    printf("6. Exit\n");
    printf("======================================\n");
    printf("Select an option: ");
}

static void print_complexity_analysis(void) {
    printf("\n=== Time Complexity: Insert New Node (n nodes) ===\n");
    printf("Insertion strategy: Binary Search Tree (BST)\n\n");
    printf("Each insert compares the new value along one root-to-leaf path.\n");
    printf("Worst case: skewed tree (height h = n-1) -> O(n)\n");
    printf("Average case: reasonably balanced BST -> O(log n)\n");
    printf("Best case: balanced tree (height h ~ log n) -> O(log n)\n\n");
    printf("Big-O summary:\n");
    printf("  Worst case:  O(n)\n");
    printf("  Average case: O(log n)\n");
    printf("  Best case:   O(log n)\n");
    printf("Space per insert: O(1) extra (one new node), excluding recursion stack O(h).\n");
    printf("================================================\n");
}

static int read_int(const char *prompt, int *out_value) {
    printf("%s", prompt);
    if (scanf("%d", out_value) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return 1;
}

static void query_node(const TreeNode *root) {
    int value = 0;
    if (!read_int("\nEnter node value to query: ", &value)) {
        return;
    }

    if (search_node(root, value) == NULL) {
        printf("Error: node %d does not exist in the tree.\n", value);
        return;
    }

    printf("\n--- Results for node %d ---\n", value);
    print_siblings(root, value);
    print_parent(root, value);
    print_grandchildren(root, value);
    printf("----------------------------\n");
}

static void insert_node_interactive(TreeNode **root) {
    int value = 0;

    if (!read_int("\nEnter new distinct value to insert: ", &value)) {
        return;
    }

    if (search_node(*root, value) != NULL) {
        printf("Error: value %d already exists in the tree.\n", value);
        return;
    }

    *root = insert_bst(*root, value);
    if (*root == NULL) {
        printf("Error: memory allocation failed during insert.\n");
        return;
    }

    printf("Successfully inserted node %d.\n", value);
}

int main(void) {
    TreeNode *root = NULL;
    int choice = 0;
    int running = 1;

    printf("=== System Component Binary Tree ===\n");
    print_insertion_rule();

    root = build_tree_from_array(get_component_array(), get_component_count());
    if (root == NULL) {
        fprintf(stderr, "Failed to construct binary tree.\n");
        return 1;
    }

    printf("Built BST from %d distinct component IDs.\n", get_component_count());
    print_root(root);

    while (running) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1:
            print_root(root);
            break;
        case 2:
            print_leaf_nodes(root);
            break;
        case 3:
            query_node(root);
            break;
        case 4:
            insert_node_interactive(&root);
            break;
        case 5:
            print_complexity_analysis();
            break;
        case 6:
            running = 0;
            printf("Exiting. Goodbye!\n");
            break;
        default:
            printf("Invalid option. Choose 1-6.\n");
            break;
        }
    }

    free_tree(root);
    return 0;
}
