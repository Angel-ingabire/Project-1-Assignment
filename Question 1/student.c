#include "student.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int append_student(StudentList *list, const Student *student) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? INITIAL_CAPACITY : list->capacity * 2;
        Student *grown = (Student *)realloc(list->records, new_capacity * sizeof(Student));
        if (grown == NULL) {
            return 0;
        }
        list->records = grown;
        list->capacity = new_capacity;
    }

    list->records[list->count++] = *student;
    return 1;
}

int read_students(const char *filename, StudentList *list) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open input file '%s'.\n", filename);
        return 0;
    }

    list->records = NULL;
    list->count = 0;
    list->capacity = 0;

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) != NULL) {
        Student student;
        int matched = sscanf(
            line,
            "%63s %63s %31s %d",
            student.first_name,
            student.last_name,
            student.student_id,
            &student.grade);

        if (matched != 4) {
            continue;
        }

        if (student.grade < 0 || student.grade > 100) {
            fprintf(stderr, "Warning: skipping invalid grade for %s %s.\n",
                    student.first_name, student.last_name);
            continue;
        }

        if (!append_student(list, &student)) {
            fclose(file);
            free_student_list(list);
            fprintf(stderr, "Error: memory allocation failed while reading records.\n");
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int write_students(const char *filename, const StudentList *list) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open output file '%s'.\n", filename);
        return 0;
    }

    for (size_t i = 0; i < list->count; i++) {
        const Student *student = &list->records[i];
        fprintf(file, "%s %s %s %d\n",
                student->first_name,
                student->last_name,
                student->student_id,
                student->grade);
    }

    fclose(file);
    return 1;
}

int compare_students(const Student *a, const Student *b) {
    if (a->grade != b->grade) {
        return a->grade - b->grade;
    }

    return strcmp(a->first_name, b->first_name);
}

static void swap_students(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(Student *arr, int low, int high) {
    Student pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare_students(&arr[j], &pivot) <= 0) {
            i++;
            swap_students(&arr[i], &arr[j]);
        }
    }

    swap_students(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_students(Student *arr, int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high);
        quick_sort_students(arr, low, pivot_index - 1);
        quick_sort_students(arr, pivot_index + 1, high);
    }
}

void free_student_list(StudentList *list) {
    free(list->records);
    list->records = NULL;
    list->count = 0;
    list->capacity = 0;
}
