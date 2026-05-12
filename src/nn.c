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