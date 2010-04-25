#include "hnet.h"

float sigmoid(float x) {
    return 1.0/(1.0 + exp(-x));
}

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
                weights[i][j][k] = ((rand() > RAND_MAX/2.0)? 1: -1) *
                    rand() / (float) RAND_MAX;
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

int hsetinputs(hnet net, float inputs[], int sig) {
    int n = net.layers[0].nneurons;
    if (sig) {
        for (int i = 0; i < n; i++) {
            net.layers[0].neurons[i].v = sigmoid(inputs[i]);
        }
    } else {
        for (int i = 0; i < n; i++) {
            net.layers[0].neurons[i].v = inputs[i];
        }
    }

    return 0;
}

int hupdatelayers(hlayer k, hlayer l, float e) {
    // i always runs over lower (k) nodes
    // j always runs over upper (l) nodes
    float **weights = k.weights;
    int n = k.nneurons;
    int m = l.nneurons;
    float sum;
    float dw[n][m];
    for (int j = 0; j < m; j++) {
        sum = 0.0;
        // get the activation of l[j]
        for (int i = 0; i < n; i++) {
            sum += weights[i][j] * k.neurons[i].v;
        }
        l.neurons[j].v = sigmoid(sum);

        // get first part of dw[i][j]
        for (int i = 0; i < n; i++) {
            dw[i][j] = e*k.neurons[i].v*l.neurons[j].v;
        }
    }

    for (int i = 0; i < n; i++) {
        sum = 0.0;
        // get the activation of reconstructed k[i]
        for (int j = 0; j < m; j++) {
            sum += weights[i][j] * l.neurons[i].v;
        }
        k.neurons[i].v = sigmoid(sum);
    }

    for (int j = 0; j < m; j++) {
        sum = 0.0;
        // get new activation of reconstructed l[j]
        for (int i = 0; i < n; i++) {
            sum += weights[i][j] * k.neurons[i].v;
        }
        l.neurons[j].v = sigmoid(sum);
        // get second part of dw[i][j]
        for (int i = 0; i < n; i++) {
            dw[i][j] -= e*k.neurons[i].v*l.neurons[j].v;
        }
    }

    return 0;
}

int hupdate(hnet net, float e) {
    int nup = net.nlayers - 1;
    for (int i = 0; i < nup; i++) {
        hupdatelayers(net.layers[i], net.layers[i+1], e);
    }
    return 0;
}

float *hreconstruction(hnet net) {
    int n = net.layers[0].nneurons;
    float *data = (float *) malloc(n*sizeof(*data));
    for (int i = 0; i < n; i++) {
        data[i] = net.layers[0].neurons[i].v;
    }
    return data;
}

