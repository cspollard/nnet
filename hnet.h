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

// creates a new hnet
hnet *hinitialize(int nlayers, int nneurons[]);

// sets the values of the neurons in layer number layer to values
int hsetvalues(hnet *net, int layer, float values[], int sig);

// updates layer number layer's weights to the layer above it
int hupdatelayer(hnet *net, int layer);

// updates all layers at once
int hupdate(hnet *net);

// reconstructs layer number layer based on layer-1
int hreconstructlayer(hnet *net, int layer);

// reconstructs all layers at once
int hreconstruct(hnet *net);

// trains layer number layer based on current data
int htrainlayer(hnet *net, int layer, float e);

// change the weights between layer and layer+1 based on current
// values of neurons in layer+1
int hbackprop(hnet *net, int layer, float e);

float *hreconstruction(hnet *net);

int hdumplayer(hnet *net, int layer);

int hdumpconnections(hnet *net);

float *hdumpneurons(hnet *net);

float *hdumpneuron(hnet *net, int n);

int hdumptofile(hnet *net, char filename[]);

hnet *hreadfromfile(char filename[]);

// sets weights between layer number layer and layer+1 to weights[]
int hsetweights(hnet *net, int layer, float weights[]);
