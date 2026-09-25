#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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

static int argmax(Matrix *m) {
    int best = 0;
    float best_val = matrix_get(m, 0, 0);
    for (int i = 1; i < m->rows; i++) {
        float val = matrix_get(m, i, 0);
        if (val > best_val) {
            best_val = val;
            best = i;
        }
    }
    return best;
}

static void evaluate(Network *net, int num_layers, Matrix *zs, Matrix *as, MNISTData *test_data, Arena *scratch) {
    int correct = 0;

    for (int i = 0; i < test_data->num_samples; i++) {
        arena_reset(scratch);

        network_forward(net, &test_data->images[i], zs, as, scratch, sigmoid);
        Matrix *output = &as[num_layers - 2];

        if (argmax(output) == argmax(&test_data->labels[i])) {
            correct++;
        }
    }

    printf("Test accuracy: %.2f%% (%d/%d)\n", (float)correct / test_data->num_samples * 100.0f, correct, test_data->num_samples);
}

static void parse_args(int argc, char **argv, int *num_epochs, float *learning_rate) {
    if (argc >= 2) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr == argv[1] || *endptr != '\0' || val <= 0) {
            fprintf(stderr, "Invalid epochs '%s', using default %d\n", argv[1], *num_epochs);
        } else {
            *num_epochs = (int)val;
        }
    }

    if (argc >= 3) {
        char *endptr;
        float val = strtof(argv[2], &endptr);
        if (endptr == argv[2] || *endptr != '\0' || val <= 0.0f || !isfinite(val)) {
            fprintf(stderr, "Invalid learning rate '%s', using default %g\n", argv[2], (double)*learning_rate);
        } else {
            *learning_rate = val;
        }
    }
}

static void train(int num_epochs, float learning_rate) {
    printf("Training: epochs=%d, learning_rate=%g\n", num_epochs, (double)learning_rate);

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

    for (int epoch = 0; epoch < num_epochs; epoch++) {
        float total_loss = 0.0f;

        for (int i = 0; i < data.num_samples; i++) {
            arena_reset(&scratch);

            network_forward(&net, &data.images[i], zs, as, &scratch, sigmoid);
            Matrix *output = &as[num_layers - 2];

            network_backward(&net, &data.images[i], output, &data.labels[i], zs, as, &scratch, sigmoid_derivative);
            network_update_weights(&net, learning_rate);

            total_loss += cross_entropy(output, &data.labels[i]);

            if ((i + 1) % 5000 == 0) {
                printf("  epoch %d: %d/%d\n", epoch, i + 1, data.num_samples);
                fflush(stdout);
            }
        }

        printf("epoch %d: avg loss %f\n", epoch, total_loss / data.num_samples);
    }

    MNISTData test_data = mnist_load(&model_a, "data/t10k-images-idx3-ubyte", "data/t10k-labels-idx1-ubyte");
    if (test_data.num_samples <= 0) {
        fprintf(stderr, "Failed to load MNIST test data, skipping evaluation\n");
    } else {
        evaluate(&net, num_layers, zs, as, &test_data, &scratch);
    }

    arena_destroy(&scratch);
    arena_destroy(&model_a);
}

int main(int argc, char **argv) {
    int num_epochs = 5;
    float learning_rate = 0.1f;

    parse_args(argc, argv, &num_epochs, &learning_rate);
    train(num_epochs, learning_rate);
    return 0;
}