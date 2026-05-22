#include "student_bst.h"

#include <stdio.h>
#include <string.h>

#define DATA_FILE "students.txt"

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int read_line(char *buffer, size_t size, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    trim_whitespace(buffer);
    return buffer[0] != '\0';
}

static void print_menu(void) {
    printf("\n========== Student Record System ==========\n");
    printf("1. Search student by last name (BST)\n");
    printf("2. Search student by last name (linear file scan)\n");
    printf("3. BST search time complexity analysis\n");
    printf("4. BST vs general binary tree\n");
    printf("5. Linear file search vs BST search\n");
    printf("6. Exit\n");
    printf("===========================================\n");
    printf("Select an option: ");
}

static void print_search_complexity(void) {
    printf("\n=== BST Search Time Complexity (n nodes) ===\n");
    printf("Search compares last names along one root-to-node path.\n");
    printf("Best case:    O(1)   - match at root\n");
    printf("Average case: O(log n) - balanced BST, height ~ log n\n");
    printf("Worst case:   O(n)   - skewed BST, height ~ n\n");
    printf("Big-O summary: best O(1), average O(log n), worst O(n)\n");
    printf("Duplicate names: O(k) extra to print k records in one node list.\n");
    printf("============================================\n");
}

static void print_bst_vs_binary_tree(void) {
    printf("\n=== Binary Search Tree vs General Binary Tree ===\n");
    printf("General Binary Tree:\n");
    printf("- Each node has up to two children with no required ordering.\n");
    printf("- Search may require visiting many nodes (often O(n)).\n\n");
    printf("Binary Search Tree (BST):\n");
    printf("- Left subtree keys < node key < right subtree keys.\n");
    printf("- Enables efficient search by eliminating half the subtree.\n");
    printf("- This program uses last name as the BST key.\n");
    printf("=================================================\n");
}

static void print_linear_vs_bst(void) {
    printf("\n=== Linear File Search vs BST Search ===\n");
    printf("Linear search from file:\n");
    printf("- Reads records sequentially until match or EOF.\n");
    printf("- Time complexity: O(n) per query (n = number of records).\n");
    printf("- Every new search may scan the entire file again.\n\n");
    printf("BST search (built once from file):\n");
    printf("- Tree built once: O(n log n) typical insert cost for n records.\n");
    printf("- Each query: O(log n) average, O(n) worst, O(1) best.\n");
    printf("- Suitable when many searches are performed on growing data.\n\n");
    printf("Conclusion: BST is more suitable for repeated lookups because it avoids\n");
    printf("scanning the whole file on every search after the initial build.\n");
    printf("========================================\n");
}

static void search_interactive_bst(const BstNode *root) {
    char last_name[MAX_NAME_LEN];
    if (!read_line(last_name, sizeof(last_name), "\nEnter last name to search: ")) {
        printf("Error: empty last name.\n");
        return;
    }
    search_by_last_name(root, last_name);
}

static void search_interactive_linear(void) {
    char last_name[MAX_NAME_LEN];
    if (!read_line(last_name, sizeof(last_name), "\nEnter last name for linear search: ")) {
        printf("Error: empty last name.\n");
        return;
    }
    search_file_linear(DATA_FILE, last_name);
}

int main(void) {
    BstNode *root = NULL;
    int choice = 0;
    int running = 1;

    printf("=== University Student Record BST System ===\n\n");
    printf("Duplicate policy: same last name -> stored in one BST node list.\n");
    printf("BST key: last name (lexicographic order).\n\n");

    if (!load_students_from_file(DATA_FILE, &root)) {
        return 1;
    }

    while (running) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1:
            search_interactive_bst(root);
            break;
        case 2:
            search_interactive_linear();
            break;
        case 3:
            print_search_complexity();
            break;
        case 4:
            print_bst_vs_binary_tree();
            break;
        case 5:
            print_linear_vs_bst();
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

    free_bst(root);
    return 0;
}
