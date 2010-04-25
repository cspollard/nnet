#include <stdio.h>
#include <time.h>
#include <cv.h>
#include "nnet.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nmax = (argc>1)? atoi(argv[1]): 5;
    int niter = (argc>2)? atoi(argv[2]): 10000;
    float g = (argc>3)? atof(argv[3]): .25;

    neuron **inputs = ninitialize(2);
    neuron **hidden = ninitialize(nmax);
    neuron **output = ninitialize(1);

    nconnect(output, hidden, 10);
    nconnect(hidden, inputs, 2);
    // printf("%p\n", inputs[0]->p);
    // fflush(stdout);

    float input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float target[4][1] = {{0}, {1}, {1}, {0}};

    int i;
    for(int j = 0; ; j++) {
        i = rand() % 4;
        // ndumpconnections(output);
        // ndumpconnections(hidden);
        ntrain(inputs, hidden, output, input[i], target[i], g, (j > niter));
        if (j > niter)
            while (getc(stdin) != '\n');
    }

    return 0;
}
