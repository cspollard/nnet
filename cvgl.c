#include "cvgl.h"

GLuint texture1; // the array for our incoming image
GLuint texture2; // the array for our reconstructed image

void convertbf(IplImage *byte_img, IplImage *float_img) {
    if ((byte_img->width != float_img->width) ||
            (byte_img->height != float_img->height) ||
            (byte_img->nChannels != float_img->nChannels) ||
            (byte_img->depth != IPL_DEPTH_8U) ||
            (float_img->depth != IPL_DEPTH_32F)) {
        printf("convertbf error.  Aborting...\n");
        exit(0);
    }
    unsigned char *runner1 = (unsigned char *) byte_img->imageDataOrigin;
    float *runner2 = (float *) float_img->imageDataOrigin;
    int skip = byte_img->widthStep - byte_img->nChannels * byte_img->width;

    for(int i = 0; i < byte_img->height; i++) {
        for(int j = 0; j < byte_img->width; j++) {
            for(int k = 0; k < byte_img->nChannels; k++) {
                *runner2++ = (float) (*runner1++);
                printf("%4.f\n", *runner2);
            }
        }
        runner1 += skip;
    }
}

void convertfb(IplImage *float_img, IplImage *byte_img) {
    if ((byte_img->width != float_img->width) ||
            (byte_img->height != float_img->height) ||
            (byte_img->nChannels != float_img->nChannels) ||
            (byte_img->depth != IPL_DEPTH_8U) ||
            (float_img->depth != IPL_DEPTH_32F)) {
        printf("convertfb error.  Aborting...\n");
        exit(0);
    }
    float *runner1 = (float *) float_img->imageDataOrigin;
    unsigned char *runner2 = (unsigned char *) byte_img->imageDataOrigin;
    int skip = float_img->widthStep - float_img->nChannels * float_img->width;

    for(int i=0; i < byte_img->height; i++) {
        for(int j=0; j < byte_img->width; j++) {
            for(int k=0; k < byte_img->nChannels; k++) {
                *runner2 = (unsigned char) *runner1;
                runner2++;
                runner1++;
            }
        }
        runner1 += skip;
    }
}

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
    IplImage *img  = cvCreateImage(cvSize(w, h), IPL_DEPTH_32F, nc);

    int n = nc*w*h;
    for (int i = 0; i < n; i++) {
        img->imageData[i] = data[i];
    }

    return img;
}

float *getimagedata(IplImage *img) {
    int n = img->width*img->height*img->nChannels;

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
    /*
	glBindTexture(GL_TEXTURE_2D, texture1);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, -1.0);
    glEnd();
    */
	glBindTexture(GL_TEXTURE_2D, texture2);
        glTexCoord2f(0.0, 0.0); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(0.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(0.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(1.0, -1.0);
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
    return 0;
}

int sdl_main() {
    setupgl();

    CvCapture *capture = cvCaptureFromCAM(1);
    if (!capture) {
        printf("Using Built-In Camera.\n");
        capture = cvCaptureFromCAM(0);
    }
    assert(capture != NULL);



    float *data;
    IplImage *recon;
    IplImage *frecon;
    IplImage *img;
    IplImage *cpy;
    IplImage *image = cvQueryFrame(capture);

    int h = image->height;
    int w = image->width;
    int nc = image->nChannels;

    int n = h*w*nc;
    int p = 4;
    int r = pow(2, 2*p);
    n /= r;

    int nneurons[] = {n, 10};
    printf("initializing neurons\n");
    fflush(stdout);
    hnet *pnet = hinitialize(2, nneurons);

    SDL_Event e;
    int i = 0;
    while (i++ < 1) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            exit(0);

        cpy = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, nc);

        cvCopyImage(image, cpy);

        img = reduce(cpy, p);
        if (!img) {
            printf("reduce error.\n");
            exit(1);
        }

        printf("madeit\n");
        fflush(stdout);
        frecon = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_32F, nc);

        convertbf(img, frecon);

        data = getimagedata(frecon);
        cvReleaseImage(&frecon);

        hsetinputs(pnet, data, 0);
        // hdumplayer(pnet, 0);
        free(data);

        hupdate(pnet, .2);

        data = hreconstruction(pnet);

        // hdumplayer(pnet, 1);

        frecon = setimagedata(data, img->width, img->height, nc);
        recon = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, nc);
        convertfb(frecon, recon);

        // loadTexture_Ipl(img, &texture1); 
        loadTexture_Ipl(recon, &texture2);
        display();

        cvReleaseImage(&cpy);
        cvReleaseImage(&recon);
        cvReleaseImage(&frecon);
        cvReleaseImage(&img);

        image = cvQueryFrame(capture);
    }
    //Free our texture
    // FreeTexture(texture1);
    FreeTexture(texture2);
	
    return 0;
}
