#include "nnet.h"


neuron **initialize(int nneurons) {
    neuron **pneurons = malloc((nneurons+1)*sizeof(*pneurons));
    for (int i = 0; i < nneurons; i++) {
        pneurons[i]->v = ((rand() > RAND_MAX)? 1: -1) *
                    (float) rand() / RAND_MAX;
    }

    pneurons[nneurons] = NULL;

    return pneurons;
}


int connect(neuron *n[], int nmax, int cmax) {
    int nconnect;
    for (int i = 0; n[i]; i++) {
        n[i]->nc = rand() % cmax;
        n[i]->c = malloc((nconnect+1)*sizeof(*(n[i]->c)));
        n[i]->w = malloc((nconnect)*sizeof(*(n[i]->w)));
        for (int j = 0; j < n[i]->nc; j++) {
            n[i]->c[j] = n[rand() % nmax];
            n[i]->w[j] = ((rand() > RAND_MAX)? 1: -1) *
                    (float) rand() / RAND_MAX;
        }
        n[i]->c[n[i]->nc] = NULL;
    }
    return 0;
}


int update(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->c[j]; j++) {
            n[i]->c[j]->v += n[i]->w[j] * n[i]->v;
        }
    }

    return 0;
}


int main(int argc, char *argv[]) {

    neuron **neurons = initialize(10);
    connect(neurons, 10, 5);
    update(neurons);
    update(neurons);
    update(neurons);

    for (int i = 0; i < 10; i++) {
        printf("%f\n", neurons[i]->v);
    }
   
    return 0;
}
