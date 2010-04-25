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

hnet *hinitialize(int nlayers, int nneurons[]);

int hdumpconnections(hnet *net);
