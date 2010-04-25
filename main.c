#include <stdio.h>
#include <time.h>
#include "cvgl.h" 

#ifndef __hnet
    #include "hnet.h"
#endif

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nneurons[5] = {10, 10, 10, 10, 10};
    hnet *pnet = hinitialize(5, nneurons);
    // hdumpconnections(*pnet);

    sdl_main();

    return 0;
}
