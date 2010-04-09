#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define __NNET 1

typedef struct neuron {
    struct neuron **c;  // children
    int nc;
    float *w;           // weights
    float v;            // value
    int i;              // index
} neuron;

int dumpconnections(neuron **n);

int update(neuron **n);

neuron **initialize(int n);

int connect(neuron *n[], int nmax, int cmax, int cmin);

int backpropagate(neuron *in, neuron *out);
