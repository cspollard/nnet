#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef __hnet
    #define __hnet 0
#endif

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

int hdumplayer(hnet *h, int layer);

int hdumpconnections(hnet *net);

int hsetinputs(hnet *net, float inputs[], int sigmoid);

int hupdatelayers(hlayer *l, hlayer *m, float e);

int hupdate(hnet *net, float e);

float *hreconstruction(hnet *net);

int hdumptofile(hnet *net, char filename[]);

int hreadfromfile(hnet *net, char filename[]);
