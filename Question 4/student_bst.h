#ifndef STUDENT_BST_H
#define STUDENT_BST_H

/*
 * DUPLICATE LAST NAME POLICY:
 * The BST key is last name (case-sensitive lexicographic order).
 * Each unique last name maps to one BST node.
 * If another student has the same last name, the record is appended to that
 * node's linked list of students (no duplicate BST nodes for the same key).
 */

#define MAX_NAME_LEN 64
#define MAX_LINE_LEN 256

typedef struct StudentRecord {
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    int grade;
    struct StudentRecord *next;
} StudentRecord;

typedef struct BstNode {
    char last_name[MAX_NAME_LEN];
    StudentRecord *records;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

int load_students_from_file(const char *filename, BstNode **root);
int insert_student(BstNode **root, const char *first, const char *last, int grade);
void search_by_last_name(const BstNode *root, const char *last_name);
int search_file_linear(const char *filename, const char *last_name);
void free_bst(BstNode *root);
void trim_whitespace(char *text);

#endif
