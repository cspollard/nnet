#include <stdio.h>
#include <time.h>
#include <cv.h>
#include "nnet.h"

int main(int argc, char *argv[]) {
    // srand(time(NULL));

    int ninputs = 100;
    int nhidden = 100;

    neuron **inputs = ninitialize(ninputs);
    neuron **hidden = ninitialize(nhidden);

    nconnect(hidden, inputs, ninputs);

    return 0;
}
