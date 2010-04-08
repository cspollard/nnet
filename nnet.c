#include "nnet.h"


neuron **initialize(int n) {
    neuron **neurons = malloc((n+1)*sizeof(*neurons));
    for (int i = 0; i < n; i++) {
        neurons[i] = malloc(sizeof(**neurons));
        neurons[i]->v = (float) rand() / RAND_MAX;
    }
    neurons[n] = NULL;
    return neurons;
}


int connect(neuron *n[], neuron *m[], int mmax) {
    for (int i = 0; n[i]; i++) {
        n[i]->p = m;
        n[i]->w = malloc(mmax)*sizeof(*(n[i]->w));
        for (int j = 0; j < mmax; j++) {
            n[i]->w[j] = ((rand() > RAND_MAX/2)? 1: -1) *
                    (float) rand() / RAND_MAX;
        }
    }
    return 0;
}


int update(neuron **n) {
    float sum;
    for (int i = 0; n[i]; i++) {
        sum = 0.0;
        for (int j = 0; n[i]->p[j]; j++) {
            sum += n[i]->w[j] * n[i]->p[j]->v;
            n[i]->p[j]->v = 1.0/(exp(sum) + 1);
        }
    }
    return 0;
}


int dumpconnections(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->p[j]; j++) {
            printf("%3d ---(%+6.3f)---> %3d\n",
                    i, n[i]->w[j], n[i]->p[j]->i);
        }
        printf("\n");
    }
    fflush(stdout);
    return 0;
}


int backpropagate(neuron *n, float values[]) {
    float err = 0.0;
    for (int i = 0; n[i]; i++) {
        float err = values[i] - n[i]->v;
        err = exp(err);
        out->w[i] += err/pow(1+err, 2) * err;
    }
    return 0;
}
