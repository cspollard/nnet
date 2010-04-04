#include "nnet.h"


neuron **initialize(int n) {
    neuron **neurons = malloc((n+1)*sizeof(*neurons));
    for (int i = 0; i < n; i++) {
        neurons[i] = malloc(sizeof(**neurons));
        neurons[i]->v = ((rand() > RAND_MAX/2)? 1: -1) *
                    (float) rand() / RAND_MAX;
        neurons[i]->i = i;
    }
    neurons[n] = NULL;
    return neurons;
}


int connect(neuron *n[], int nmax, int cmax, int cmin) {
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


int update(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->c[j]; j++) {
            n[i]->c[j]->v += n[i]->w[j] * n[i]->v;
        }
    }

    return 0;
}


int dumpconnections(neuron **n) {
    for (int i = 0; n[i]; i++) {
        for (int j = 0; n[i]->c[j]; j++) {
            printf("\t%3d ---(%6.3f)---> %3d\n",
                    i, n[i]->w[j], n[i]->c[j]->i);
        }
        printf("\n");
    }
    fflush(stdout);

    return 0;
}


int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nmax = (argc>1)? atoi(argv[1]): 100;
    int cmax = (argc>2)? atoi(argv[2]): nmax/2;
    int cmin = (argc>3)? atoi(argv[3]): 0;
    int niter = (argc>4)? atoi(argv[4]): 100;
    int njter = (argc>5)? atoi(argv[5]): 100;

    neuron **neurons = initialize(nmax);
    connect(neurons, nmax, cmax, cmin);
    printf("madeit!\n");
    fflush(stdout);
    dumpconnections(neurons);
    // printf("%.3f\n", neurons[nmax-1]->v);

    for (int i = 0; i < niter; i++) {
        for (int j = 0; j < njter; j++) {
            neurons[0]->v = 1;
            update(neurons);
        }
        //printf("neuron %d: %.3f\n", nmax-1, neurons[nmax-1]->v);
    }

    return 0;
}
