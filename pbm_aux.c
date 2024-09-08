// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - NAVID AZIMI


#include "pbm.h"
#include <stdlib.h>
#include <stdio.h>


#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


PPMImage * new_ppmimage(unsigned int width, unsigned int height, unsigned int max) {
    PPMImage * image = malloc(sizeof * image);
    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    image->max = max;
    image->width = width;
    image->height = height;
    for (int i=0; i<3; i++) {
        int h=0;
        image->pixmap[i] = malloc(height * sizeof(unsigned int *));
        while (h<height) {
            image->pixmap[i][h] = malloc(width * sizeof(unsigned int));
            h++;
        }
    }
    return image;
}

PBMImage * new_pbmimage( unsigned int width, unsigned int height ) {
    PBMImage * image = malloc(sizeof * image);
    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    int h=0;
    image->width = width;
    image->height = height;
    image->pixmap = malloc(height * sizeof(unsigned int *));
    while (h<height) {
        image->pixmap[h] = malloc(width * sizeof(unsigned int));
        h++;
    }
    return image;
}

PGMImage * new_pgmimage( unsigned int width, unsigned int height, unsigned int max) {
    PGMImage * image = malloc(sizeof * image);
    if (image == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    int h=0;
    image->max = max;
    image->width = width;
    image->height = height;
    image->pixmap = malloc(height * sizeof(unsigned int *));
    while (h<height) {
        image->pixmap[h] = malloc(width * sizeof(unsigned int));
        h++;
    }
    return image;
}


void del_ppmimage(PPMImage * image) {
    int i=0;
    while (i<3) {
        for (int h=0; h < image->height; h++) {
            free(image->pixmap[i][h]);
        }
        free(image->pixmap[i]);
        i++;
    }
    free(image);
}

void del_pgmimage( PGMImage * image) {
    int h=0;
    while (h < image->height) {
        free(image->pixmap[h]);
        h++;
    }
    free(image->pixmap);
    free(image);
}

void del_pbmimage( PBMImage * image) {
    int h=0;
    while (h < image->height) {
        free(image->pixmap[h]);
        h++;
    }
    free(image->pixmap);
    free(image);
}
