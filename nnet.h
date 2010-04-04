#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct neuron {
    struct neuron **c;
    int nc;
    float *w;
    float v;
    int i;  // index
} neuron;
