#include "student_bst.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim_whitespace(char *text) {
    size_t start = 0;
    size_t end;
    size_t i;

    if (text == NULL || text[0] == '\0') {
        return;
    }

    while (text[start] != '\0' && isspace((unsigned char)text[start])) {
        start++;
    }

    if (start > 0) {
        for (i = 0; text[start + i] != '\0'; i++) {
            text[i] = text[start + i];
        }
        text[i] = '\0';
    }

    end = strlen(text);
    while (end > 0 && isspace((unsigned char)text[end - 1])) {
        text[end - 1] = '\0';
        end--;
    }
}

static int compare_last_names(const char *a, const char *b) {
    return strcmp(a, b);
}

static StudentRecord *create_record(const char *first, const char *last, int grade) {
    StudentRecord *record = (StudentRecord *)malloc(sizeof(StudentRecord));
    if (record == NULL) {
        return NULL;
    }

    strncpy(record->first_name, first, MAX_NAME_LEN - 1);
    record->first_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(record->last_name, last, MAX_NAME_LEN - 1);
    record->last_name[MAX_NAME_LEN - 1] = '\0';
    record->grade = grade;
    record->next = NULL;
    return record;
}

static int append_record(StudentRecord **head, const char *first, const char *last, int grade) {
    StudentRecord *record = create_record(first, last, grade);
    StudentRecord *current;

    if (record == NULL) {
        return 0;
    }

    if (*head == NULL) {
        *head = record;
        return 1;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = record;
    return 1;
}

static BstNode *create_bst_node(const char *last_name, StudentRecord *record) {
    BstNode *node = (BstNode *)malloc(sizeof(BstNode));
    if (node == NULL) {
        return NULL;
    }

    strncpy(node->last_name, last_name, MAX_NAME_LEN - 1);
    node->last_name[MAX_NAME_LEN - 1] = '\0';
    node->records = record;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int insert_student(BstNode **root, const char *first, const char *last, int grade) {
    int cmp;
    StudentRecord *record;

    if (root == NULL || first == NULL || last == NULL || last[0] == '\0') {
        return 0;
    }

    if (*root == NULL) {
        record = create_record(first, last, grade);
        if (record == NULL) {
            return 0;
        }
        *root = create_bst_node(last, record);
        return *root != NULL;
    }

    cmp = compare_last_names(last, (*root)->last_name);
    if (cmp < 0) {
        return insert_student(&(*root)->left, first, last, grade);
    }
    if (cmp > 0) {
        return insert_student(&(*root)->right, first, last, grade);
    }

    return append_record(&(*root)->records, first, last, grade);
}

static int parse_line(const char *line, char *first, char *last, int *grade) {
    char buffer[MAX_LINE_LEN];
    char *token;
    char *rest;
    int field = 0;

    if (line == NULL || first == NULL || last == NULL || grade == NULL) {
        return 0;
    }

    strncpy(buffer, line, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';

    rest = buffer;
    while ((token = strtok(rest, "|")) != NULL && field < 3) {
        trim_whitespace(token);
        if (field == 0) {
            strncpy(first, token, MAX_NAME_LEN - 1);
            first[MAX_NAME_LEN - 1] = '\0';
        } else if (field == 1) {
            strncpy(last, token, MAX_NAME_LEN - 1);
            last[MAX_NAME_LEN - 1] = '\0';
        } else {
            *grade = atoi(token);
        }
        field++;
        rest = NULL;
    }

    return field == 3 && first[0] != '\0' && last[0] != '\0';
}

int load_students_from_file(const char *filename, BstNode **root) {
    FILE *file;
    char line[MAX_LINE_LEN];
    char first[MAX_NAME_LEN];
    char last[MAX_NAME_LEN];
    int grade = 0;
    int line_number = 0;
    int loaded = 0;
    int skipped = 0;

    if (root == NULL) {
        return 0;
    }

    *root = NULL;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'.\n", filename);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        trim_whitespace(line);

        if (line[0] == '\0') {
            continue;
        }

        if (!parse_line(line, first, last, &grade)) {
            fprintf(stderr, "Warning: malformed record at line %d (skipped).\n", line_number);
            skipped++;
            continue;
        }

        if (grade < 0 || grade > 100) {
            fprintf(stderr, "Warning: invalid grade at line %d (skipped).\n", line_number);
            skipped++;
            continue;
        }

        if (!insert_student(root, first, last, grade)) {
            fprintf(stderr, "Error: memory allocation failed at line %d.\n", line_number);
            fclose(file);
            free_bst(*root);
            *root = NULL;
            return 0;
        }
        loaded++;
    }

    fclose(file);

    if (loaded == 0) {
        fprintf(stderr, "Error: file '%s' is empty or contains no valid records.\n", filename);
        return 0;
    }

    if (skipped > 0) {
        printf("Loaded %d record(s); skipped %d invalid line(s).\n", loaded, skipped);
    } else {
        printf("Loaded %d student record(s) into BST.\n", loaded);
    }

    return 1;
}

static BstNode *find_node(const BstNode *root, const char *last_name) {
    int cmp;

    if (root == NULL || last_name == NULL) {
        return NULL;
    }

    cmp = compare_last_names(last_name, root->last_name);
    if (cmp == 0) {
        return (BstNode *)root;
    }
    if (cmp < 0) {
        return find_node(root->left, last_name);
    }
    return find_node(root->right, last_name);
}

static void print_records(const StudentRecord *records) {
    const StudentRecord *current = records;
    int index = 1;

    while (current != NULL) {
        printf("  Record %d: %s | %s | %d\n",
               index,
               current->first_name,
               current->last_name,
               current->grade);
        current = current->next;
        index++;
    }
}

void search_by_last_name(const BstNode *root, const char *last_name) {
    BstNode *node;

    if (last_name == NULL || last_name[0] == '\0') {
        printf("Error: last name cannot be empty.\n");
        return;
    }

    node = find_node(root, last_name);
    if (node == NULL) {
        printf("No student found with last name '%s'.\n", last_name);
        return;
    }

    printf("Student record(s) for last name '%s':\n", last_name);
    print_records(node->records);
}

int search_file_linear(const char *filename, const char *last_name) {
    FILE *file;
    char line[MAX_LINE_LEN];
    char first[MAX_NAME_LEN];
    char last[MAX_NAME_LEN];
    int grade = 0;
    int found = 0;
    int line_number = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'.\n", filename);
        return -1;
    }

    printf("\nLinear file search results for '%s':\n", last_name);
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        trim_whitespace(line);
        if (line[0] == '\0') {
            continue;
        }
        if (!parse_line(line, first, last, &grade)) {
            continue;
        }
        if (strcmp(last, last_name) == 0) {
            printf("  Line %d: %s | %s | %d\n", line_number, first, last, grade);
            found++;
        }
    }

    fclose(file);
    if (found == 0) {
        printf("  No matching records found in file.\n");
    }
    return found;
}

static void free_records(StudentRecord *records) {
    StudentRecord *current = records;
    while (current != NULL) {
        StudentRecord *next = current->next;
        free(current);
        current = next;
    }
}

void free_bst(BstNode *root) {
    if (root == NULL) {
        return;
    }
    free_bst(root->left);
    free_bst(root->right);
    free_records(root->records);
    free(root);
}
