#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <ctype.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef __hnet
    #include "hnet.h"
#endif

int setupgl();

int sdl_main();

float *getimagedata(IplImage *img, int n);

IplImage *setimagedata(float *data, int n);
