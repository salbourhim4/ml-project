#include <stdlib.h>
#include "nn.h"
#include <math.h>

// Activation functions and derivatives

float relu(float x) {
    if (x > 0.0) {
        return x;
    }
    else {
        return 0.0;
    }
}

float sigmoid(float x) {
    return 1 / (1 + exp(-x));
}

float relu_derivative(float x) {
    if (x > 0.0) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

float sigmoid_derivative(float x) {
    return (exp(-x)) / pow((1 + exp(-x)), 2);
}

Network network_create(Arena *a, int *layer_sizes, int num_layers) {
    Network n = {0};
    n.num_layers = num_layers;

    n.weights = arena_alloc(a, (num_layers - 1) * sizeof(Matrix));
    n.biases = arena_alloc(a, (num_layers - 1) * sizeof(Matrix));
    n.gradients = arena_alloc(a, (num_layers - 1) * sizeof(Matrix));

    for (int i = 0; i < num_layers - 1; i++) {
        n.weights[i] = matrix_create(a, layer_sizes[i], layer_sizes[i+1]);
        n.biases[i] = matrix_create(a, 1, layer_sizes[i+1]);
        n.gradients[i] = matrix_create(a, layer_sizes[i], layer_sizes[i+1]);

        matrix_randomize(&n.weights[i], -1.0, 1.0);
        matrix_zero(&n.biases[i]);
        matrix_zero(&n.gradients[i]);
    }

    return n;
}

void network_forward(Network *net, Matrix *input, Arena *scratch, float (*activation)(float)) {
    for (int i = 0; i < net->num_layers - 1; i++) {
        Matrix z = matrix_create(scratch, net->weights[i].cols, 1);
        Matrix a = matrix_create(scratch, net->weights[i].cols, 1);
        
        matrix_mul(&net->weights[i], input, &z);
        matrix_add(&z, &net->biases[i], &z);
        
        for (int j = 0; j < z.rows*z.cols; j++) {
            float val = matrix_get(&z, j, 0);
            matrix_set(&a, j, 0, activation(val));
        }
        
        input = &a;
    }
}

void network_backward(Network *net, Matrix *output, Matrix *labels, float (*derivative)(float)) {
    
}
