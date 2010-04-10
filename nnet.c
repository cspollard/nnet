#include "nnet.h"


neuron **ninitialize(int n) {
    neuron **neurons = malloc((n+1)*sizeof(*neurons));
    for (int i = 0; i < n; i++) {
        neurons[i] = malloc(sizeof(**neurons));
        neurons[i]->v = (float) rand() / RAND_MAX;
        neurons[i]->i = i;
    }
    neurons[n] = NULL;
    return neurons;
}


int nconnect(neuron *n[], int nmax, int cmax, int cmin) {
    for (int i = 0; n[i]; i++) {
        n[i]->nc = (cmax>cmin)? (rand() % (cmax-cmin)) + cmin: cmax;
        n[i]->c = malloc((n[i]->nc+1)*sizeof(*(n[i]->c)));
        n[i]->w = malloc((n[i]->nc)*sizeof(*(n[i]->w)));
        for (int j = 0; j < n[i]->nc; j++) {
            n[i]->c[j] = n[rand() % nmax];
            n[i]->w[j] = ((rand() > RAND_MAX/2)? 1: -1) *
                    (float) rand() / RAND_MAX;
        }
        n[i]->c[n[i]->nc] = NULL;
    }
    return 0;
}


int nupdate(neuron **n) {
    float sum;
    for (int i = 0; n[i]; i++) {
        sum = 0.0;
        for (int j = 0; n[i]->c[j]; j++) {
            sum += n[i]->w[j] * n[i]->c[j]->v;
        }
        n[i]->v = 1.0/(exp(sum) + 1);
    }
    return 0;
}


int nrandupdate(neuron **n, int nmax) {
    float sum = 0;
    neuron *pn = n[rand() % nmax];
    for (int j = 0; pn->c[j]; j++) {
        sum += pn->w[j] * pn->c[j]->v;
    }
    pn->v = 1.0/(exp(sum) +1);
    return 0;
}


int ndumpconnections(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->c[j]; j++) {
            printf("%3d ---(%+6.3f)---> %3d\n",
                    i, n[i]->w[j], n[i]->c[j]->i);
        }
        printf("\n");
    }
    fflush(stdout);
    return 0;
}


int nbackpropagate(neuron *in, neuron *out) {
    for (int i = 0; out->c[i]; i++) {
        float w = out->c[i]->v - in->v;
        float x = exp(w);
        out->w[i] += x/pow(x+1, 2)*w;
    }
    return 0;
}
