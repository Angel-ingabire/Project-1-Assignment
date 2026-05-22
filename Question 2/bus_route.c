#include "bus_route.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
static void delay_seconds(unsigned int seconds) {
    Sleep(seconds * 1000U);
}
#else
#include <unistd.h>
static void delay_seconds(unsigned int seconds) {
    sleep(seconds);
}
#endif

BusStopNode *create_stop(const char *name, int number) {
    BusStopNode *node = (BusStopNode *)malloc(sizeof(BusStopNode));
    if (node == NULL) {
        return NULL;
    }

    strncpy(node->name, name, MAX_STOP_NAME - 1);
    node->name[MAX_STOP_NAME - 1] = '\0';
    node->number = number;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

int insert_stop_at_end(BusRoute *route, const char *name, int number) {
    BusStopNode *node = create_stop(name, number);
    if (node == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        return 0;
    }

    if (route->head == NULL) {
        route->head = node;
        route->tail = node;
    } else {
        node->prev = route->tail;
        route->tail->next = node;
        route->tail = node;
    }

    route->count++;
    return 1;
}

static void print_stop(const BusStopNode *stop) {
    printf("  Stop #%d: %s\n", stop->number, stop->name);
}

void traverse_forward(const BusRoute *route) {
    if (route->head == NULL) {
        printf("No bus stops on this route.\n");
        return;
    }

    printf("\n--- Forward journey (home -> campus) ---\n");
    BusStopNode *current = route->head;

    while (current != NULL) {
        print_stop(current);

        if (current->next != NULL) {
            printf("  (waiting 3 seconds...)\n");
            delay_seconds(3);
        }

        current = current->next;
    }

    printf("Reached the last bus stop. Journey complete.\n\n");
}

void traverse_backward(const BusRoute *route) {
    if (route->tail == NULL) {
        printf("No bus stops on this route.\n");
        return;
    }

    printf("\n--- Backward journey (campus -> home) ---\n");
    BusStopNode *current = route->tail;

    while (current != NULL) {
        print_stop(current);

        if (current->prev != NULL) {
            printf("  (waiting 3 seconds...)\n");
            delay_seconds(3);
        }

        current = current->prev;
    }

    printf("Reached the first bus stop. Journey complete.\n\n");
}

void print_route(const BusRoute *route) {
    if (route->head == NULL) {
        printf("Route is empty.\n");
        return;
    }

    printf("\nCurrent route (%lu stop(s)):\n", (unsigned long)route->count);
    BusStopNode *current = route->head;
    while (current != NULL) {
        print_stop(current);
        current = current->next;
    }
    printf("\n");
}

void free_route(BusRoute *route) {
    BusStopNode *current = route->head;
    while (current != NULL) {
        BusStopNode *next = current->next;
        free(current);
        current = next;
    }

    route->head = NULL;
    route->tail = NULL;
    route->count = 0;
}

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return 1;
}

int read_initial_route(BusRoute *route) {
    route->head = NULL;
    route->tail = NULL;
    route->count = 0;

    int stop_count = 0;
    printf("Enter the number of initial bus stops: ");
    if (scanf("%d", &stop_count) != 1 || stop_count <= 0) {
        fprintf(stderr, "Invalid number of stops.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    char name[MAX_STOP_NAME];
    int number = 0;

    for (int i = 0; i < stop_count; i++) {
        printf("\nBus stop %d of %d\n", i + 1, stop_count);
        printf("  Enter stop name: ");
        if (!read_line(name, sizeof(name)) || name[0] == '\0') {
            fprintf(stderr, "Invalid stop name.\n");
            return 0;
        }

        printf("  Enter stop number: ");
        if (scanf("%d", &number) != 1) {
            fprintf(stderr, "Invalid stop number.\n");
            clear_input_buffer();
            return 0;
        }
        clear_input_buffer();

        if (!insert_stop_at_end(route, name, number)) {
            return 0;
        }
    }

    return 1;
}
