#include <stdlib.h>
#include "matrix.h"

#include <studdef.h>

Arena arena_create(size_t bytes) {
    Arena a = {0};
    a.start = (char *)malloc(bytes);
    if (a.start) {
        a.capacity = bytes;
        a.head = a.start;
    }
    return a;
}

void *arena_alloc(Arena *a, size_t bytes) {
    // check for overflow
    if (a->head + bytes > a->start + a->capacity) {
        return NULL;
    }

    void *ptr = a->head;
    a->head += bytes;
    return ptr;
}

void arena_reset(Arena *a) {
    a->head = a->start;
}

void arena_destroy(Arena *a) {
    free(a->start);      // hand the memory back to the OS
    a->start = NULL;     
    a->head = NULL;
    a->capacity = 0;  
}