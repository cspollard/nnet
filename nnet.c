#include "nnet.h"


neuron **ninitialize(int n) {
    neuron **neurons = malloc((n+1)*sizeof(*neurons));
    for (int i = 0; i < n; i++) {
        neurons[i] = malloc(sizeof(**neurons));
        neurons[i]->v = (float) rand() / RAND_MAX;
        neurons[i]->p = NULL;
    }
    neurons[n] = NULL;
    return neurons;
}


int nconnect(neuron *n[], neuron *p[], int pmax) {
    for (int i = 0; n[i]; i++) {
        n[i]->p = p;
        n[i]->w = malloc((pmax)*sizeof(*(n[i]->w)));
        for (int j = 0; p[j]; j++) {
            n[i]->w[j] = ((rand() > RAND_MAX/2)? 1: -1) *
                    (float) rand() / RAND_MAX;
        }
    }
    return 0;
}


int nupdate(neuron **n) {
    float sum;
    for (int i = 0; n[i]; i++) {
        sum = 0.0;
        for (int j = 0; n[i]->p[j]; j++) {
            sum += n[i]->w[j] * n[i]->p[j]->v;
            n[i]->v = 1.0/(exp(sum) + 1);
        }
    }
    return 0;
}

int ndumpconnections(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->p[j]; j++) {
            printf("%3d ---(%+6.3f)---> %3d\n",
                    i, n[i]->w[j], j);
        }
        printf("\n");
    }
    fflush(stdout);
    return 0;
}


float nfinderr(neuron **n, float values[]) {
    float sum = 0.0;
    for (int i = 0; n[i]; i++) {
        n[i]->err = n[i]->v - values[i];
        sum += n[i]->err;
    }
    return sum*sum;
}


int nbackpropagate(neuron **n, float g) {
    if (n[0]->p == NULL) {
        return 0;
    }

    neuron **p = n[0]->p;
    float s;
    for (int i = 0; p[i]; i++) {
        p[i]->err = 0.0;
        for (int j = 0; n[j]; j++) {
            s = n[j]->v * (1 - n[j]->v);
            p[i]->err += s * n[j]->err * n[j]->w[i];
            n[j]->w[i] += g * n[j]->err * p[i]->v;
        }
    }
    nbackpropagate(p, g);
    
    return 0;
}


int ntrain(neuron **inputs, neuron **hidden, neuron **output,
        float input[], float target[], float g, int debug) {

    for (int i = 0; inputs[i]; i++) {
        inputs[i]->v = input[i];
    }

    nupdate(hidden);
    nupdate(output);
    float err;
    err = nfinderr(output, target);
    nbackpropagate(output, g);

    if (debug) {
        printf("Input: %.3f %.3f, Target: %.3f, Output %.3f, Energy: %.3f\n",
            input[0], input[1], target[0], output[0]->v, err);
    }

    return 0;
}
