#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#ifndef __NNET
    #include "nnet.h"
#endif

int setupgl();

int draw(neuron **n, int w, int h);
