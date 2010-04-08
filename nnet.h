#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct neuron {
    struct neuron **p;  // parents
    float *w;           // weights
    float v;            // value
} neuron;

int dumpconnections(neuron **n);

int update(neuron **n);

neuron **initialize(int n);

int connect(neuron *n[], int nmax, int cmax, int cmin);

int backpropagate(neuron *in, neuron *out);
