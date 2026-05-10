#include <stdio.h>
#include "matrix.h"

int main(void) {
    Arena a = arena_create(1024);

    Matrix a_mat = matrix_create(&a, 2, 3);
    matrix_set(&a_mat, 0, 0, 1); matrix_set(&a_mat, 0, 1, 2); matrix_set(&a_mat, 0, 2, 3);
    matrix_set(&a_mat, 1, 0, 4); matrix_set(&a_mat, 1, 1, 5); matrix_set(&a_mat, 1, 2, 6);

    Matrix b_mat = matrix_create(&a, 3, 2);
    matrix_set(&b_mat, 0, 0, 7); matrix_set(&b_mat, 0, 1, 8);
    matrix_set(&b_mat, 1, 0, 9); matrix_set(&b_mat, 1, 1, 10);
    matrix_set(&b_mat, 2, 0, 11); matrix_set(&b_mat, 2, 1, 12);

    Matrix result = matrix_create(&a, 2, 2);
    matrix_mul(&a_mat, &b_mat, &result);

    printf("Result:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%.1f ", matrix_get(&result, i, j));
        }
        printf("\n");
    }

    arena_destroy(&a);
    return 0;
}