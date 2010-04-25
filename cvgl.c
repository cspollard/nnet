//#include <windows.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <ctype.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "hnet.h"
#include "cvgl.h"

GLuint texture1; // the array for our incoming image
GLuint texture2; // the array for our reconstructed image

IplImage *reduce(IplImage *img) {
    IplImage *tmp1 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_32F, 3);
    IplImage *tmp2 = cvCreateImage(cvSize(160, 120), IPL_DEPTH_32F, 3);
    cvPyrDown(img, tmp1, CV_GAUSSIAN_5x5);
    cvPyrDown(tmp1, tmp2, CV_GAUSSIAN_5x5);
    cvReleaseImage(&tmp1);
    return tmp2;
}



IplImage *setimagedata(float *data, int n) {
    IplImage *img  = cvCreateImage(cvSize(640, 480), IPL_DEPTH_32F, 3);

    for (int i = 0; i < n; i++) {
        img->imageData[i] = data[i];
    }

    return img;
}

float *getimagedata(IplImage *img, int n) {
    float *data = (float *) malloc(n*sizeof(*data));
    if (!data) return NULL;

    for (int i = 0; i < n; i++) {
        data[i] = img->imageData[i];
    }

    return data;
}

//The IplImage to OpenGl texture function
int loadTexture_Ipl(IplImage *image, GLuint *text) {

    if (image == NULL)
        return -1;
   
    glGenTextures(1, text);
    //bind the texture to it's array
	glBindTexture(GL_TEXTURE_2D, *text);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height,
            0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);

    return 0;
}

void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();  
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
	glBindTexture(GL_TEXTURE_2D, texture1);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, -1.0);
    glEnd();
	glBindTexture(GL_TEXTURE_2D, texture2);
        glTexCoord2f(0.0, 0.0); glVertex2f(1.0, 2.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(0.0, 2.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(0.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(1.0, 0.0);
    glEnd();
    SDL_GL_SwapBuffers();
}

void FreeTexture(GLuint texture) {
  glDeleteTextures(1, &texture);
}

int reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    return 0;
}

int setupgl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
     
    atexit(SDL_Quit);

    SDL_Surface *screen;
     
    screen = SDL_SetVideoMode(1200, 450, 16, SDL_DOUBLEBUF | SDL_OPENGL);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption("hello", "HELLO");

    glViewport(0, 0, 1200, 450);

    CvCapture *capture = cvCaptureFromCAM(1);
    if (!capture) {
        printf("Using Built-In Camera.\n");
        capture = cvCaptureFromCAM(0);
    }
    assert(capture != NULL);

    int n = 160*120*3;

    int nneurons[] = {n, n};
    printf("initializing neurons\n");
    fflush(stdout);
    hnet *pnet = hinitialize(2, nneurons);

    IplImage *image = cvQueryFrame(capture);
    printf("reducing image size");
    IplImage *img = reduce(image);

    float *data = getimagedata(img, n);
    hsetinputs(pnet, data, 0);
    printf("setting inputs.\n");
    fflush(stdout);
    hupdate(pnet, .3);
    free(data);
    data = hreconstruction(pnet);
    printf("reconstructing.\n");
    fflush(stdout);

    IplImage *recon = setimagedata(data, n);
	
    SDL_Event e;
    while (1) {
        // load the iplimages to the opengl textures
        loadTexture_Ipl(img, &texture1); 
        loadTexture_Ipl(recon, &texture2);

        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);

        display();

        image = cvQueryFrame(capture);
        printf("reducing image size");
        IplImage *img = reduce(image);
        float *data = getimagedata(image, n);
        printf("setting inputs.\n");
        fflush(stdout);
        hsetinputs(pnet, data, 0);
        hupdate(pnet, .2);
        free(data);
        printf("reconstructing.\n");
        fflush(stdout);
        data = hreconstruction(pnet);
        cvReleaseImage(&recon);
        IplImage *recon = setimagedata(data, n);
    }
    //Free our texture
    FreeTexture(texture1);
    FreeTexture(texture2);
	
    return 0;
}

