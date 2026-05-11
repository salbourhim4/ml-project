#ifndef NN_H
#define NN_H

#include <stddef.h>

typedef struct {
    Matrix *weights;      // array of weight matrices
    Matrix *biases;       // array of bias vectors
    Matrix *gradients;    // array of gradient matrices
    int num_layers;
} Network;