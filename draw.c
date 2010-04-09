#include "draw.h"


int setupgl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        fflush(stdout);
        return 1;
    }
     
    atexit(SDL_Quit);

    SDL_Surface *screen;
     
    screen = SDL_SetVideoMode(800, 800, 16,
            SDL_DOUBLEBUF | SDL_OPENGL | SDL_RESIZABLE);

    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption("nnet", "nnet");

    glViewport(0, 0, 800, 800);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);

    return 0;
}


int ndraw(neuron **n, int w, int h) {
    w = w? w: 1;
    h = h? h: 1;
    float dx = 1.5 / w;
    float dy = 1.5 / h;
    float x = -.75;
    float y = -.75;
    float c;

    for (int i = 0; n[i]; i++) {
        c = n[i]->v;
        glColor3f(c, c, c);
        glBegin(GL_QUADS);
        glVertex3f(x, y, 0);
        glVertex3f(x+dx, y, 0);
        glVertex3f(x+dx, y+dy, 0);
        glVertex3f(x, y+dy, 0);
        glEnd();

        if (!((i+1) % w)) {
            y += dy;
            x = -.75;
        } else
            x += dx;
    }

    glFlush();
    SDL_GL_SwapBuffers();
    return 0;
}
