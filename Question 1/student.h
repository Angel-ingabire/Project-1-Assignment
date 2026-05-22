#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

#define MAX_NAME_LEN 64
#define MAX_ID_LEN 32
#define MAX_LINE_LEN 256
#define INITIAL_CAPACITY 16

typedef struct {
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char student_id[MAX_ID_LEN];
    int grade;
} Student;

typedef struct {
    Student *records;
    size_t count;
    size_t capacity;
} StudentList;

int read_students(const char *filename, StudentList *list);
int write_students(const char *filename, const StudentList *list);
void quick_sort_students(Student *arr, int low, int high);
int compare_students(const Student *a, const Student *b);
void free_student_list(StudentList *list);

#endif
