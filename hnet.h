#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct hneuron {
    float v;
    float *weights;
} hneuron;

typedef struct hlayer {
    int nneurons;
    hneuron *neurons;
    float **weights;
} hlayer;

typedef struct hnet {
    int nlayers;
    hlayer *layers;
    float ***weights;
} hnet;

float sigmoid(float x);

hnet *hinitialize(int nlayers, int nneurons[]);

int hdumpconnections(hnet net);

int hsetinputs(hnet net, float inputs[], int sigmoid);

int hupdatelayers(hlayer l, hlayer m, float e);

int hupdate(hnet net);
