#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    int row;
    int col;
    float *data;
} Matrix;

typedef struct {
    char *start;
    char *head;
    size_t capacity;
} Arena; 

Arena  arena_create(size_t bytes);
void  *arena_alloc(Arena *a, size_t bytes);
void   arena_reset(Arena *a);
void   arena_destroy(Arena *a);

Matrix matrix_create(Arena *a, int rows, int cols);
float matrix_get(Matrix *m, int i, int j);
void matrix_set(Matrix *m, int i, int j, float val);
void matrix_add(Matrix *a, Matrix *b, Matrix *result);
void matrix_sub(Matrix *a, Matrix *b, Matrix *result);
void matrix_mul(Matrix *a, Matrix *b, Matrix *result);
void matrix_scale(Matrix *m, float scalar, Matrix *result);
void matrix_transpose(Matrix *m, Matrix *result);
void matrix_elementwise_mul(Matrix *a, Matrix *b, Matrix *result);  // element wise multiplication multiplies the elements in the same position of two matrices
void matrix_zero(Matrix *m);
void matrix_copy(Matrix *src, Matrix *dst);
void matrix_randomize(Matrix *m, float min, float max);


#endif