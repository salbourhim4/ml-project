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

// Matrix Operations

Matrix matrix_create(Arena *a, int rows, int cols) {
    Matrix m = {0};
    float *data = arena_alloc(a,  rows * cols * sizeof(float));
    if (!data) {
        return m;   // m is {0} so m.data will be NULL, caller can check for it
    }
    m.rows = rows;
    m.cols = cols;
    m.data = data;
    return m;
}

float matrix_get(Matrix *m, int i, int j) {
    int pos = i * m->cols + j;
    return m->data[pos];
}

void matrix_set(Matrix *m, int i, int j, float val) {
    int pos = i * m->cols + j;
    m->data[pos] = val;
}

void matrix_add(Matrix *a, Matrix *b, Matrix *result) { // assuming both are n x n matrices 
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            float num = matrix_get(a, i, j) + matrix_get(b, i, j);
            matrix_set(result, i, j, num);
        }
    }
}

void matrix_sub(Matrix *a, Matrix *b, Matrix *result) {
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            float num = matrix_get(a, i, j) - matrix_get(b, i, j);
            matrix_set(result, i, j, num);
        }
    }
}