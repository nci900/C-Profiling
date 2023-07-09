#ifndef LIST_H
#define LIST_H

#include <stddef.h> // For size_t

typedef struct List {
    size_t len;
    size_t capacity;
    double* data;
} List;

List list_init(void);
void list_destroy(List*);
void list_reserve(List*, const size_t);

void list_append(List*, const double);
void list_insert(List*, const size_t, const double);
double list_pop(List*, const size_t);

void list_print(List);
void list_print_contents(List);

#endif
