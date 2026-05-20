#ifndef NN_H
#define NN_H

#include <stddef.h>
#include "matrix.h"

typedef struct {
    Matrix *weights;      // array of weight matrices
    Matrix *biases;       // array of bias vectors
    Matrix *weight_gradients;    // array of weight gradient matrices
    Matrix *bias_gradients; 
    int num_layers;
} Network;

float relu(float x);
float sigmoid(float x);
float relu_derivative(float x);
float sigmoid_derivative(float x);

Network network_create(Arena *a, int *layer_sizes, int num_layers);
void network_forward(Network *net, Matrix *input, Matrix *zs, Matrix *as, Arena *scratch, float (*activation)(float));

// let each aspect of backprop be its own function instead of writing all of them in network_backward
Matrix compute_dE(Matrix *output, Matrix *labels, Arena *scratch);
Matrix compute_dZ(Matrix *error, Matrix *z, Arena *scratch, float (*derivative)(float));
Matrix compute_dW(Matrix *dZ, Matrix *input, Arena *scratch);
Matrix compute_dA(Matrix *weights, Matrix *dZ, Arena *scratch);

void network_backward(Network *net, Matrix *input, Matrix *output, Matrix *labels, Matrix *zs, Matrix *as, Arena *scratch, float (*derivative)(float));//Labels from mnist
void network_update_weights(Network *net, float learning_rate);
void network_free(Network *n);
float cross_entropy(Matrix *output, Matrix *labels);

#endif