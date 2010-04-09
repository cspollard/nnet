#include <stdio.h>
#include <time.h>
#include "nnet.h"
#include "draw.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nmax = (argc>1)? atoi(argv[1]): 100;
    int cmax = (argc>2)? atoi(argv[2]): nmax/2;
    int cmin = (argc>3)? atoi(argv[3]): 0;
    int niter = (argc>4)? atoi(argv[4]): 100;
    int njter = (argc>5)? atoi(argv[5]): 100;

    neuron **neurons = initialize(nmax);
    connect(neurons, nmax, cmax, cmin);
    dumpconnections(neurons);
    // printf("%.3f\n", neurons[nmax-1]->v);

    for (int i = 0; i < niter; i++) {
        for (int j = 0; j < njter; j++) {
            neurons[0]->v = 1;
            update(neurons);
            draw(neurons, nmax/10, 10);
            // backpropagate(neurons[0], neurons[nmax-1]);
        }
        // printf("neuron %d: %.3f\n", nmax-1, neurons[nmax-1]->v);
    }

    return 0;
}
