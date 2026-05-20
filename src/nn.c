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
    n.weight_gradients = arena_alloc(a, (num_layers - 1) * sizeof(Matrix));
    n.bias_gradients = arena_alloc(a, (num_layers - 1) * sizeof(Matrix));

    for (int i = 0; i < num_layers - 1; i++) {
        n.weights[i] = matrix_create(a, layer_sizes[i], layer_sizes[i+1]);
        n.biases[i] = matrix_create(a, 1, layer_sizes[i+1]);
        n.weight_gradients[i] = matrix_create(a, layer_sizes[i], layer_sizes[i+1]);
        n.bias_gradients[i] = matrix_create(a, 1, layer_sizes[i+1]);

        matrix_randomize(&n.weights[i], -1.0, 1.0);
        matrix_zero(&n.biases[i]);
        matrix_zero(&n.weight_gradients[i]);
        matrix_zero(&n.bias_gradients[i]);
    }
    return n;
}

void network_forward(Network *net, Matrix *input, Matrix *zs, Matrix *as, Arena *scratch, float (*activation)(float)) {
    for (int i = 0; i < net->num_layers - 1; i++) {
        Matrix z = matrix_create(scratch, net->weights[i].rows, 1);
        Matrix a = matrix_create(scratch, net->weights[i].rows, 1);
        
        matrix_mul(&net->weights[i], input, &z);
        matrix_add(&z, &net->biases[i], &z);
        zs[i] = z;

        for (int j = 0; j < z.rows*z.cols; j++) {
            float val = matrix_get(&z, j, 0);
            matrix_set(&a, j, 0, activation(val));
        }

        as[i] = a;     
        input = &a;
    }
}

Matrix compute_dE(Matrix *output, Matrix *labels, Arena *scratch) {
    Matrix dE = matrix_create(scratch, output->rows, 1);
    matrix_sub(output, labels, &dE);
    return dE;
}

Matrix compute_dZ(Matrix *error, Matrix *z, Arena *scratch, float (*derivative)(float)) {
    Matrix dZ = matrix_create(scratch, z->rows, 1);

    for (int j = 0; j < z->rows; j++) {
        float d = derivative(matrix_get(z, j, 0));
        float e = matrix_get(error, j, 0);
        matrix_set(&dZ, j, 0, d * e);
    }
    return dZ;
}

Matrix compute_dW(Matrix *dZ, Matrix *input, Arena *scratch) {
    Matrix dW = matrix_create(scratch, dZ->rows, input->rows);
    Matrix input_transpose = matrix_create(scratch, input->cols, input->rows);
    matrix_transpose(input, &input_transpose);
    matrix_mul(dZ, &input_transpose, &dW);
    return dW;
}

Matrix compute_dA(Matrix *weights, Matrix *dZ, Arena *scratch) {
    Matrix dA = matrix_create(scratch, weights->cols, 1);
    Matrix weights_transpose = matrix_create(scratch, weights->cols, weights->rows);
    matrix_transpose(weights, &weights_transpose);
    matrix_mul(&weights_transpose, dZ, &dA);
    return dA;
}

void network_backward(Network *net, Matrix *input, Matrix *output, Matrix *labels, Matrix *zs, Matrix *as, Arena *scratch, float (*derivative)(float)) {
    Matrix dE = compute_dE(output, labels, scratch);
    for (int i = net->num_layers - 2; i >= 0; i--) {
        Matrix *layer_input;
        if (i == 0) {
            layer_input = input;
        }
        else {
            layer_input = &as[i-1];
        }
        Matrix dZ = compute_dZ(&dE, &zs[i], scratch, derivative);
        Matrix dW = compute_dW(&dZ, layer_input, scratch);
        matrix_copy(&dW, &net->weight_gradients[i]);
        matrix_copy(&dZ, &net->bias_gradients[i]);
        dE = compute_dA(&net->weights[i], &dZ, scratch);
    }
}

void network_update_weights(Network *net, float learning_rate) {
    for (int i = 0; i < net->num_layers - 1 ; i++) {
        matrix_scale(&net->weight_gradients[i], learning_rate, &net->weight_gradients[i]);
        matrix_sub(&net->weights[i], &net->weight_gradients[i], &net->weights[i]);

        matrix_scale(&net->bias_gradients[i], learning_rate, &net->bias_gradients[i]);
        matrix_sub(&net->biases[i], &net->bias_gradients[i], &net->biases[i]);
    }
}

float cross_entropy(Matrix *output, Matrix *labels) {
    float loss = 0.0;
    for (int i = 0; i < output->rows; i++) {
        loss += matrix_get(labels, i, 0) * log(matrix_get(output, i, 0));
    }
    return -1 * loss; 
}

