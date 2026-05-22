#include "student.h"

#include <stdio.h>

int main(void) {
    const char *input_file = "students.txt";
    const char *output_file = "sorted_students.txt";

    StudentList students;

    if (!read_students(input_file, &students)) {
        return 1;
    }

    if (students.count == 0) {
        fprintf(stderr, "Warning: no valid student records found in '%s'.\n", input_file);
    } else if (students.count > 1) {
        quick_sort_students(students.records, 0, (int)students.count - 1);
    }

    if (!write_students(output_file, &students)) {
        free_student_list(&students);
        return 1;
    }

    printf("Sorted %lu student record(s) into '%s'.\n",
           (unsigned long)students.count, output_file);
    free_student_list(&students);
    return 0;
}
