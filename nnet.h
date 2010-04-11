#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct neuron {
    struct neuron **p;  // parents
    float *w;           // weights
    float v;            // value
    float err;          // error
} neuron;

int ndumpconnections(neuron **n);

int nupdate(neuron **n);

neuron **ninitialize(int n);

int nconnect(neuron *n[], neuron *p[], int pmax);

int nbackpropagate(neuron **n, float g);

float nfinderr(neuron **n, float values[]);

int ntrain(neuron **input, neuron **hidden, neuron **output,
        float inputs[], float target[], float g, int debug);
