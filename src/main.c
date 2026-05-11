#include <stdio.h>
#include "matrix.h"

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%.2f ", matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    Arena a = arena_create(1024 * 1024);

    // test matrices
    Matrix m1 = matrix_create(&a, 2, 2);
    matrix_set(&m1, 0, 0, 1); matrix_set(&m1, 0, 1, 2);
    matrix_set(&m1, 1, 0, 3); matrix_set(&m1, 1, 1, 4);

    Matrix m2 = matrix_create(&a, 2, 2);
    matrix_set(&m2, 0, 0, 5); matrix_set(&m2, 0, 1, 6);
    matrix_set(&m2, 1, 0, 7); matrix_set(&m2, 1, 1, 8);

    Matrix result = matrix_create(&a, 2, 2);

    // test add
    matrix_add(&m1, &m2, &result);
    printf("Add (expect 6 8 / 10 12):\n");
    print_matrix(&result);

    // test sub
    matrix_sub(&m2, &m1, &result);
    printf("Sub (expect 4 4 / 4 4):\n");
    print_matrix(&result);

    // test mul
    matrix_mul(&m1, &m2, &result);
    printf("Mul (expect 19 22 / 43 50):\n");
    print_matrix(&result);

    // test scale
    matrix_scale(&m1, 2.0, &result);
    printf("Scale (expect 2 4 / 6 8):\n");
    print_matrix(&result);

    // test transpose
    Matrix t = matrix_create(&a, 2, 2);
    matrix_transpose(&m1, &t);
    printf("Transpose (expect 1 3 / 2 4):\n");
    print_matrix(&t);

    // test elementwise mul
    matrix_elementwise_mul(&m1, &m2, &result);
    printf("Elementwise (expect 5 12 / 21 32):\n");
    print_matrix(&result);

    // test zero
    matrix_zero(&result);
    printf("Zero (expect 0 0 / 0 0):\n");
    print_matrix(&result);

    // test copy
    matrix_copy(&m1, &result);
    printf("Copy (expect 1 2 / 3 4):\n");
    print_matrix(&result);

    // test randomize
    matrix_randomize(&result, -1.0, 1.0);
    printf("Randomize (expect values between -1 and 1):\n");
    print_matrix(&result);

    arena_destroy(&a);
    return 0;
}