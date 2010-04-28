#include "cvgl.h"

GLuint texture1; // the array for our reconstructed image

IplImage *reduce(IplImage *img, int n) {
    if (!n) {
        return img;
    }

    int h = img->height;
    int w = img->width;
    if (h%2 || w%2) {
        printf("Illegal width/height.\n");
        return NULL;
    }
    h /= 2;
    w /= 2;

    int depth = img->depth;
    int nChannels = img->nChannels;
    IplImage *tmp1;
    IplImage *tmp2;

    tmp1 = cvCreateImage(cvSize(w, h), depth, nChannels);

    cvPyrDown(img, tmp1, CV_GAUSSIAN_5x5);

    tmp2 = reduce(tmp1, n-1);
    if (n != 1)
        cvReleaseImage(&tmp1);

    return tmp2;
}

IplImage *setimagedata(float *data, int w, int h, int nc)  {
    IplImage *img  = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nc);

    int l, m, n;
    for (int i = 0; i < img->height; i++) {
        l = i * img->widthStep;
        m = i * img->width * img->nChannels;
        for (int j = 0; j < img->width; j++) {
            int n = j * img->nChannels;
            for (int k = 0; k < img->nChannels; k++) {
                img->imageData[l+n+k] = (unsigned char) (data[m+n+k] * 255.0);
                // printf("data[%d]: %f\n", m+n+k, data[m+n+k]);
                // printf("imageData[%d]: %u\n", l+n+k, (unsigned char) img->imageData[m+n+k]);
                // fflush(stdout);
            }
        }
    }
    return img;
}

float *getimagedata(IplImage *img) {
    int n = img->width * img->height * img->nChannels;

    float *data = (float *) malloc(n*sizeof(*data));
    if (!data) return NULL;

    // printf("img w, h = %d, %d\n", img->width, img->height);
    int l, m;
    for (int i = 0; i < img->height; i++) {
        l = i * img->widthStep;
        m = i * img->width * img->nChannels;
        for (int j = 0; j < img->width; j++) {
            int n = j * img->nChannels;
            for (int k = 0; k < img->nChannels; k++) {
                data[m+n+k] = (unsigned char) img->imageData[l+n+k] / 255.0;
                // printf("imageData[%d]: %u\n", l+n+k, (unsigned char) img->imageData[m+n+k]);
                // printf("data[%d]: %f\n", m+n+k, data[m+n+k]);
                // fflush(stdout);
            }
        }
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
        glTexCoord2f(0.0, 0.0); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(1.0, -1.0);
    glEnd();
    SDL_GL_SwapBuffers();
}

void FreeTexture(GLuint texture) {
    printf("help!\n");
    fflush(stdout);
    glDeleteTextures(1, &texture);
}

int setupgl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
     
    atexit(SDL_Quit);

    SDL_Surface *screen;
     
    screen = SDL_SetVideoMode(600, 450, 16, SDL_DOUBLEBUF | SDL_OPENGL);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption("hello", "HELLO");

    glViewport(0, 0, 600, 450);
    return 0;
}

int recon_loop() {
    CvCapture *capture = cvCaptureFromCAM(1);
    if (!capture) {
        printf("Using Built-In Camera.\n");
        capture = cvCaptureFromCAM(0);
    }
    assert(capture != NULL);
    float *data;
    int p = 4;

    IplImage *image = cvQueryFrame(capture);
    int h = image->height;
    int w = image->width;
    int nc = image->nChannels;

    IplImage *cpy = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nc);
    cvCopyImage(image, cpy);
    IplImage *img = reduce(cpy, p);
    IplImage *recon;

    int n = img->width*img->height*img->nChannels;
    int nneurons[] = {n, 100, 500};
    hnet *pnet = hinitialize(3, nneurons);
    cvReleaseImage(&cpy);
    cvReleaseImage(&img);
    SDL_Event e;
    int i = 0;

    while (i < 100) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);

        cpy = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nc);

        cvCopyImage(image, cpy);

        img = reduce(cpy, p);
        data = getimagedata(img);
        hsetinputs(pnet, data, 0);
        // hdumplayer(pnet, 0);
        cvReleaseImage(&cpy);
        free(data);

        hupdate(pnet, .1);
        // hdumplayer(pnet, 0);
        data = hreconstruction(pnet);

        recon = setimagedata(data, img->width, img->height, nc);
        free(data);

        loadTexture_Ipl(recon, &texture1);
        display();

        cvReleaseImage(&recon);
        cvReleaseImage(&img);
        image = cvQueryFrame(capture);
        printf("\rIteration %d", i);
        fflush(stdout);
        i++;
    }
    printf("\n");
    hdumptofile(pnet, "asdf.hnet");
    FreeTexture(texture1);
	
    return 0;
}

int ndump_loop() {
    CvCapture *capture = cvCaptureFromCAM(1);
    if (!capture) {
        printf("Using Built-In Camera.\n");
        capture = cvCaptureFromCAM(0);
    }
    assert(capture != NULL);
    float *data;
    int p = 4;

    IplImage *image = cvQueryFrame(capture);
    int h = image->height;
    int w = image->width;
    int nc = image->nChannels;

    IplImage *cpy = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nc);
    cvCopyImage(image, cpy);
    IplImage *img = reduce(cpy, p);
    cvReleaseImage(&cpy);
    IplImage *recon;

    hnet *pnet = malloc(sizeof(*pnet));
    hreadfromfile(pnet, "asdf.hnet");

    SDL_Event e;
    int i = 0;
    data = hdumpneurons(pnet);
    recon = setimagedata(data, img->width, img->height, nc);
    free(data);

    loadTexture_Ipl(recon, &texture1);
    display();
    printf("Total Network\n");
    SDL_Delay(5000);
    while (i < pnet->layers[pnet->nlayers-1].nneurons) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);

        data = hdumpneuron(pnet, i);

        recon = setimagedata(data, img->width, img->height, nc);
        free(data);

        loadTexture_Ipl(recon, &texture1);
        display();

        cvReleaseImage(&recon);

        printf("\rNeuron %d", i);
        fflush(stdout);

        SDL_Delay(100);
        i++;
    }
    cvReleaseImage(&img);
    FreeTexture(texture1);
	
    return 0;
}


int sdl_main() {
    setupgl();

    ndump_loop();
    return 0;
}
