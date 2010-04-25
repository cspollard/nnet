//#include <windows.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <ctype.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

GLuint texture; //the array for our texture
GLfloat angle = 0.0;

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
	glBindTexture(GL_TEXTURE_2D, texture);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, -1.0);
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

    IplImage *image = cvQueryFrame(capture);
	
    SDL_Event e;
    while (1) {
        //The load iplimage to opengl texture
        loadTexture_Ipl(image, &texture); 

        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);

        display();
        image = cvQueryFrame(capture);
    }
    //Free our texture
    FreeTexture(texture);
	
    return 0;
} 
