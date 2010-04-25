#include <stdio.h>
#include <time.h>
// #include "webcam.h" 
#include "hnet.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int nneurons[5] = {10, 10, 10, 10, 10};
    hnet *pnet = hinitialize(5, nneurons);
    hdumpconnections(*pnet);

    return 0;
}
