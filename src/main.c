#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "mnist.h"
#include "nn.h"

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%.2f ", matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

static void train(void) {
    srand((unsigned)time(NULL));

    // Model arena: sized for the full 60k-image MNIST training set
    // (~184MiB of images/labels/network) plus headroom.
    Arena model_a = arena_create(256 * 1024 * 1024);
    // Scratch arena: reset every sample; holds one sample's forward/backward
    // temporaries (peak usage is dominated by the first-layer dW/dA matrices,
    // roughly 1MB per sample at this architecture).
    Arena scratch = arena_create(4 * 1024 * 1024);

    MNISTData data = mnist_load(&model_a, "data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte");
    if (data.num_samples <= 0) {
        fprintf(stderr, "Failed to load MNIST training data\n");
        arena_destroy(&scratch);
        arena_destroy(&model_a);
        return;
    }

    int layer_sizes[] = {784, 128, 64, 10};
    int num_layers = sizeof(layer_sizes) / sizeof(layer_sizes[0]);
    Network net = network_create(&model_a, layer_sizes, num_layers);

    Matrix *zs = arena_alloc(&model_a, (num_layers - 1) * sizeof(Matrix));
    Matrix *as = arena_alloc(&model_a, (num_layers - 1) * sizeof(Matrix));

    // TODO: training loop (next pass)

    arena_destroy(&scratch);
    arena_destroy(&model_a);
}

int main(void) {
    train();
    return 0;
}