#include "bus_route.h"

#include <stdio.h>

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    size_t len = 0;
    while (buffer[len] != '\0') {
        len++;
    }
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return 1;
}

static void print_menu(void) {
    printf("\n========== Bus Navigation Menu ==========\n");
    printf("1. Start forward journey (home -> campus)\n");
    printf("2. Start backward journey (campus -> home)\n");
    printf("3. Add a new bus stop at the end of the route\n");
    printf("4. View current route\n");
    printf("5. Exit\n");
    printf("==========================================\n");
    printf("Select an option: ");
}

static void add_stop_interactive(BusRoute *route) {
    char name[MAX_STOP_NAME];
    int number = 0;

    printf("\nEnter new stop name: ");
    clear_input_buffer();
    if (!read_line(name, sizeof(name)) || name[0] == '\0') {
        printf("Could not add stop: invalid name.\n");
        return;
    }

    printf("Enter new stop number: ");
    if (scanf("%d", &number) != 1) {
        printf("Could not add stop: invalid number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (insert_stop_at_end(route, name, number)) {
        printf("Bus stop added successfully.\n");
    }
}

int main(void) {
    BusRoute route;
    int choice = 0;
    int running = 1;

    printf("=== Public Bus Route Navigator ===\n\n");

    if (!read_initial_route(&route)) {
        fprintf(stderr, "Failed to build initial route.\n");
        return 1;
    }

    print_route(&route);

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
            traverse_forward(&route);
            break;
        case 2:
            traverse_backward(&route);
            break;
        case 3:
            add_stop_interactive(&route);
            break;
        case 4:
            print_route(&route);
            break;
        case 5:
            running = 0;
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid option. Please choose 1-5.\n");
            break;
        }
    }

    free_route(&route);
    return 0;
}
