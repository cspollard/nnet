#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define __NNET

typedef struct neuron {
    struct neuron **c;  // children
    int nc;
    float *w;           // weights
    float v;            // value
    int i;              // index
} neuron;

int ndumpconnections(neuron **n);

int nupdate(neuron **n);

int nrandupdate(neuron **n, int nmax);

neuron **ninitialize(int n);

int nconnect(neuron *n[], int nmax, int cmax, int cmin);

int nbackpropagate(neuron *in, neuron *out);
