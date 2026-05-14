#ifndef NN_H
#define NN_H

#include <stddef.h>
#include "matrix.h"

typedef struct {
    Matrix *weights;      // array of weight matrices
    Matrix *biases;       // array of bias vectors
    Matrix *gradients;    // array of gradient matrices
    int num_layers;
} Network;

float relu(float x);
float sigmoid(float x);
float relu_derivative(float x);
float sigmoid_derivative(float x);

Network network_create(Arena *a, int *layer_sizes, int num_layers);
void network_forward(Network *net, Matrix *input, Matrix *zs, Arena *scratch, float (*activation)(float));
void network_backward(Network *net, Matrix *output, Matrix *labels, Matrix *zs, Arena *scratch, float (*derivative)(float));//Labels from mnist
void network_update_weights(Network *net, float learning_rate);
void network_free(Network *n);
float cross_entropy(Matrix *output, Matrix *labels);

#endif