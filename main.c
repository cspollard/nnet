#include <stdio.h>
#include <time.h>
// #include "webcam.h" 
#include "hnet.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nneurons[2] = {5, 5};
    hnet *pnet = hinitialize(2, nneurons);
    hdumpconnections(*pnet);

    return 0;
}
