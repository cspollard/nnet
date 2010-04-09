#include <draw.h>


int setupgl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
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


int draw(neuron **n, int w, int h) {
    float dx = 1.0 / w;
    float dy = 1.0 / h;
    float x = 0.0;
    float y = 0.0;
    float c;
    for (int i = 0; n[i]; i++) {
        c = n[i]->v;

        glColor3f(c, c, c);
        glBegin(GL_QUAD);
        glVertex3f(x, y, 0);
        glVertex3f(x+dx, y, 0);
        glVertex3f(x, y+dy, 0);
        glVertex3f(x+dx, y+dy, 0);
        glEnd();

        x += dx;
        y += dy;
    }
    glFlush();
    SDL_GL_SwapBuffers();
    return 0;
}
