#include <stdio.h>
#include <time.h>
#include "nnet.h"
#include "draw.h"

int main(int argc, char *argv[]) {
    setupgl();

    srand(time(NULL));

    int nmax = (argc>1)? atoi(argv[1]): 100;
    int w  = (int) sqrt(nmax);
    int h = w + 1;
    int cmax = (argc>2)? atoi(argv[2]): nmax/2;
    int cmin = (argc>3)? atoi(argv[3]): 0;
    int niter = (argc>4)? atoi(argv[4]): 100;
    int njter = (argc>5)? atoi(argv[5]): 100;

    neuron **neurons = ninitialize(nmax);
    nconnect(neurons, nmax, cmax, cmin);
    ndraw(neurons, w, h);
    SDL_Delay(50);

    SDL_Event e;
    for (int i = 0; i < niter; i++) {
        for (int j = 0; j < njter; j++) {
            nupdate(neurons);
            ndraw(neurons, w, h);
            SDL_PollEvent(&e);
            if (e.type == SDL_QUIT)
                exit(0);
            SDL_Delay(100);
        }
    }

    return 0;
}
