#include <stdlib.h>
#include <stdio.h>

typedef struct neuron {
    struct neuron **c;
    int nc;
    float *w;
    float v;
} neuron;
