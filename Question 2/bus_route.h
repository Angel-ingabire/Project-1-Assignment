#ifndef BUS_ROUTE_H
#define BUS_ROUTE_H

#include <stddef.h>

#define MAX_STOP_NAME 64

typedef struct BusStopNode {
    char name[MAX_STOP_NAME];
    int number;
    struct BusStopNode *next;
    struct BusStopNode *prev;
} BusStopNode;

typedef struct {
    BusStopNode *head;
    BusStopNode *tail;
    size_t count;
} BusRoute;

BusStopNode *create_stop(const char *name, int number);
int insert_stop_at_end(BusRoute *route, const char *name, int number);
void traverse_forward(const BusRoute *route);
void traverse_backward(const BusRoute *route);
void print_route(const BusRoute *route);
void free_route(BusRoute *route);
int read_initial_route(BusRoute *route);

#endif
