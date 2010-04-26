#include <stdio.h>
#include <time.h>
#include "cvgl.h" 

#ifndef __hnet
    #include "hnet.h"
#endif

int main(int argc, char *argv[]) {
    srand(time(NULL));

    sdl_main();

    return 0;
}
