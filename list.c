#include "list.h"   // For declarations. Note double quotes for project file, rather than <>
#include <stdio.h>  // For printf()
#include <stdlib.h> // For realloc, free


List list_init(void) {
    return (List) {.len = 0, .data = NULL};
}



void list_destroy(List* list) {
    if (list->data != NULL)
        free(list->data);
    list->data = NULL; // Null out data pointer for easier detection of use-after-free bugs
    list->len = 0;
}



void list_append(List* list, const double value) {
    list->data = (double*)realloc(list->data, sizeof(double) * (list->len + 1));
    list->data[list->len] = value;
    list->len++;
}


void list_insert(List* list, const size_t index, const double val) {
  list->data = (double*)realloc(list->data, sizeof(double) * (list->len + 1));
  for (double* item = list->data + list->len; item > list->data + index; --item)
    *item = *(item - 1);
  list->len++;
  list->data[index] = val;
}


double list_pop(List* list, const size_t index) {
  double* item = &list->data[index];
  double out = *item;
  for (; item < list->data + list->len - 1; ++item)
    *item = *(item+1);
  list->len--;
  list->data = (double*)realloc(list->data, sizeof(double)*list->len);
  return out;
}


void list_print(List list) {
    printf("{\n\tlen: %ld\n\tdata: %p\n}\n", list.len, (void*)list.data);
}



void list_print_contents(List list) {
    printf("[");

    // Loop through pointers to each element of the list
    for (double* v = list.data; v < list.data + list.len; ++v) {
        // Print ", " before all elements except the first.
        if (v != list.data)
            printf(", %g", *v);
        else
            printf("%g", *v);
    }

    printf("]\n");
}
