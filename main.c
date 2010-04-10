#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "nnet.h"
#include "draw.h"

int main(int argc, char *argv[]) {
    setupgl();

    pthread_t *threads;
    pthread_attr_t pattr;

    srand(time(NULL));

    int nmax = (argc>1)? atoi(argv[1]): 100;
    int w  = 1 + (int) sqrt(nmax);
    int h = w;
    int cmax = (argc>2)? atoi(argv[2]): nmax/2;
    int cmin = (argc>3)? atoi(argv[3]): 0;
    int niter = (argc>4)? atoi(argv[4]): 100;
    int njter = (argc>5)? atoi(argv[5]): 100;

    threads = malloc(njter*sizeof(*threads));
    pthread_attr_init(&pattr);

    neuronptr *p;
    neuron **neurons = ninitialize(nmax);
    
    printf("%p\n", neurons);
    printf("%p\n", neurons);
    fflush(stdout);

    (*p).n = neurons;
    printf("%p\n", neurons);
    printf("%p\n", neurons);
    p->nmax = nmax;
    nconnect(neurons, nmax, cmax, cmin);
    ndraw(neurons, w, h);
    SDL_Delay(50);

    SDL_Event e;
    for (int i = 0; i < niter; i++) {
        for (int j = 0; j < njter; j++) {
            pthread_create(&threads[j], &pattr,
                    (void *) nrandupdate, (void *) p);
        }
        ndraw(neurons, w, h);
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);
        SDL_Delay(100);
    }

    return 0;
}
