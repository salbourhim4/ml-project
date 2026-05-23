#ifndef MNIST_H
#define MNIST_H

#include <stdint.h>
#include "matrix.h"

typedef struct {
    Matrix *images; // array of num_samples matrices, each 784x1
    Matrix *labels; // array of num_samples matrices, each 10x1
    int num_samples;
} MNISTData;

// Used to swap a 4 byte big endian integer to a little endian
uint32_t swap_endian(uint32_t val);

// Load images and labels from MNIST binary files into arena-allocated matrices.
// Returns a MNISTData with num_samples set to 0 on failure.
MNISTData mnist_load(Arena *a, const char *image_path, const char *label_path);

#endif
