#include "hnet.h"

hnet *hinitialize(int nlayers, int nneurons[]) {
    hnet *pnet = (hnet *) malloc(sizeof(*pnet));
    hlayer *layers = (hlayer *) malloc((nlayers+1)*sizeof(*layers));
    if (!pnet || !layers) return NULL;

    int n, m;
    hneuron *neurons;
    for (int i = 0; i < nlayers; i++) {
        n = nneurons[i];
        neurons = (hneuron *) malloc(n*sizeof(*neurons));
        if (!neurons) return NULL;

        for (int j = 0; j < n; j++)
            neurons[j].v = 0.0;

        layers[i].neurons = neurons;
        layers[i].nneurons = n;
    }

    float ***weights = (float ***) malloc(nlayers*sizeof(*weights));
    if (!weights) return NULL;

    for (int i = 0; i < nlayers-1; i++) {
        n = nneurons[i];
        m = nneurons[i+1];

        weights[i] = (float **) malloc((n+1)*sizeof(*(weights[i])));
        if (!weights[i]) return NULL;

        for (int j = 0; j < n; j++) {
            weights[i][j] = (float *)
                malloc((m+1)*sizeof(*(weights[i][j])));
            if (!weights[i][j]) return NULL;

            for (int k = 0; k < m; k++) {
                weights[i][j][k] = rand() / (float) RAND_MAX;
            }
            layers[i].neurons[j].weights = weights[i][j];
        }
        weights[i][n] = NULL;
        layers[i].weights = weights[i];
    }
    weights[nlayers] = NULL;

    pnet->nlayers = nlayers;
    pnet->layers = (hlayer *) layers;
    pnet->weights = weights;

    return pnet;
}

int hdumpconnections(hnet net) {
    int n, m, l;
    
    n = net.nlayers;
    for (int i = 0; i < n-1; i++) {
        m = net.layers[i].nneurons;
        printf("Layer %d\n------------------\n", i);
        for (int j = 0; j < m; j++) {
            l = net.layers[i+1].nneurons;
            printf("Neuron %d\n------------------\n", j);
            for (int k = 0; k < l; k++) {
                printf("Connection %d: %.4f\n", k,
                        net.layers[i].neurons[j].weights[k]);
            }
        }
    }
    return 0;
}
