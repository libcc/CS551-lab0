// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - NAVID AZIMI


#include "pbm.h"
#include "string.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
typedef enum {false, true} bool;


// CVT Functions
PBMImage * convert_ppm_to_pbm(PPMImage * ppm) {
    PBMImage * pbm = new_pbmimage(ppm->width, ppm->height);
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width) {
            pbm->pixmap[h][w] = ((ppm->pixmap[0][h][w] + ppm->pixmap[1][h][w] + ppm->pixmap[2][h][w]) / 3.0) < (ppm->max / 2.0);
            w++;
        }
    }
    return pbm;
}

PGMImage * convert_ppm_to_pgm(PPMImage * ppm, unsigned int pgm_max) {
    PGMImage * pgm = new_pgmimage(ppm->width, ppm->height, pgm_max);
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width) {
            pgm->pixmap[h][w] = (((ppm->pixmap[0][h][w] + ppm->pixmap[1][h][w] + ppm->pixmap[2][h][w]) / 3.0) / ppm->max) * pgm->max;
            w++;
        }
    }
    return pgm;
}

void sepia_transform_inplace(PPMImage * ppm) {
    unsigned int New_R, New_G, New_B;
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width) {
            New_R = 0.393 * ppm->pixmap[0][h][w] + 0.769 * ppm->pixmap[1][h][w] + 0.189 * ppm->pixmap[2][h][w];
            ppm->pixmap[0][h][w] = min(New_R, ppm->max);
            New_G = 0.349 * ppm->pixmap[0][h][w] + 0.686 * ppm->pixmap[1][h][w] + 0.168 * ppm->pixmap[2][h][w];
            ppm->pixmap[1][h][w] = min(New_G, ppm->max);
            New_B = 0.272 * ppm->pixmap[0][h][w] + 0.534 * ppm->pixmap[1][h][w] + 0.131 * ppm->pixmap[2][h][w];
            ppm->pixmap[2][h][w] = min(New_B, ppm->max);
            w++;
        }
    }
}

void remove_RGB_channel_inplace(PPMImage * ppm, const char * channel) {
    int channel_idx;
    if (strcmp(channel, "red") == 0)
        channel_idx = 0;
    else if (strcmp(channel, "green") == 0)
        channel_idx = 1;
    else if (strcmp(channel, "blue") == 0)
        channel_idx = 2;
    else {
        fprintf(stderr, "ERROR: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", channel);
        exit(1);
    }
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width) {
            ppm->pixmap[channel_idx][h][w] = 0;
            w++;
        }
    }
}

void isolate_RGB_channel_inplace(PPMImage * ppm, const char * channel) {
    if (strcmp(channel, "red") == 0) {
        remove_RGB_channel_inplace(ppm, "green");
        remove_RGB_channel_inplace(ppm, "blue");
    } else if (strcmp(channel, "green") == 0) {
        remove_RGB_channel_inplace(ppm, "red");
        remove_RGB_channel_inplace(ppm, "blue");
    } else if (strcmp(channel, "blue") == 0) {
        remove_RGB_channel_inplace(ppm, "red");
        remove_RGB_channel_inplace(ppm, "green");
    }
    else {
        fprintf(stderr, "ERROR: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", channel);
        exit(1);
    }
}

void mirror_vertically_inplace(PPMImage * ppm) {
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width/2) {
            ppm->pixmap[0][h][ppm->width-w-1] = ppm->pixmap[0][h][w];
            ppm->pixmap[1][h][ppm->width-w-1] = ppm->pixmap[1][h][w];
            ppm->pixmap[2][h][ppm->width-w-1] = ppm->pixmap[2][h][w];
            w++;
        }
    }
}

PPMImage * thumnail_ppm(PPMImage * ppm, unsigned int scale) {
    unsigned int width_thumbnail = ppm->width/scale;
    unsigned int height_thumbnail = ppm->height/scale;
    if (ppm->width % scale) width_thumbnail++;
    if (ppm->height % scale) height_thumbnail++;
    PPMImage * ppm_thumbnail = new_ppmimage(width_thumbnail, height_thumbnail, ppm->max);
    for (int h=0; h<ppm_thumbnail->height; h++) {
        int w=0;
        while (w<ppm_thumbnail->width) {
            ppm_thumbnail->pixmap[0][h][w] = ppm->pixmap[0][h*scale][w*scale];
            ppm_thumbnail->pixmap[1][h][w] = ppm->pixmap[1][h*scale][w*scale];
            ppm_thumbnail->pixmap[2][h][w] = ppm->pixmap[2][h*scale][w*scale];
            w++;
        }
    }
    return ppm_thumbnail;
}

void nup_ppm_inplace(PPMImage * ppm, unsigned int scale) {
    unsigned int w_nup = ppm->width/scale;
    unsigned int h_nup = ppm->height/scale;
    if (ppm->width % scale) w_nup++;
    if (ppm->height % scale) h_nup++;
    for (int h=0; h<ppm->height; h++) {
        int w=0;
        while (w<ppm->width) {
            if (h<h_nup && w<w_nup) {
                ppm->pixmap[0][h][w] = ppm->pixmap[0][h*scale][w*scale];
                ppm->pixmap[1][h][w] = ppm->pixmap[1][h*scale][w*scale];
                ppm->pixmap[2][h][w] = ppm->pixmap[2][h*scale][w*scale];
            } else {
                ppm->pixmap[0][h][w] = ppm->pixmap[0][h%h_nup][w%w_nup];
                ppm->pixmap[1][h][w] = ppm->pixmap[1][h%h_nup][w%w_nup];
                ppm->pixmap[2][h][w] = ppm->pixmap[2][h%h_nup][w%w_nup];
            }
            w++;
        }
    }
}


// Getopt Parser
int main (char argc, char *argv[]) {
    char * input_filename = NULL;
    char * output_filename = NULL;
    char cvt_f = 'b';
    bool transformed = false;
    long ps_factor;
    extern char *optarg;
    extern int optind;
    int opt;
    char * channel = NULL;
    char * cptr;
    
    while ((opt = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1) {
        switch (opt) {
            case 'b':
            case 'g':
                if (transformed) {
                    fprintf(stderr, "Error: Multiple transformations specified\n");
                    exit(1);
                }
                cvt_f = opt;
                transformed = true;
                ps_factor = strtol(optarg, &cptr, 10);
                if (ps_factor > 65535) {
                    fprintf(stderr, "Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optarg);
                    exit(1);
                }
                break;
            
            case 'i':
            case 'r':
                if (transformed) {
                    fprintf(stderr, "Error: Multiple transformations specified\n");
                    exit(1);
                }
                cvt_f = opt;
                transformed = true;
                channel = optarg;
                if (channel == NULL || (strcmp(channel, "red") && strcmp(channel, "green") && strcmp(channel, "blue"))) {
                    fprintf(stderr, "ERROR: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", channel);
                    exit(1);
                }
                break;
                
            case 's':
            case 'm':
                if (transformed) {
                    fprintf(stderr, "Error: Multiple transformations specified\n");
                    exit(1);
                }
                cvt_f = opt;
                transformed = true;
                break;
            
            case 't':
            case 'n':
                if (transformed) {
                    fprintf(stderr, "Error: Multiple transformations specified\n");
                    exit(1);
                }
                cvt_f = opt;
                transformed = true;
                ps_factor = strtol(optarg, &cptr, 10);
                if (ps_factor < 1 || ps_factor > 8) {
                    fprintf(stderr, "ERROR: Invalid scale factor: %s; must be 1-8\n", optarg);
                    exit(1);
                }
                break;
            case 'o':
                output_filename = optarg;
                break;
            case '?':
                fprintf(stderr, "SYNOPSIS: ppmcvt [bg:i:r:smt:n:o:] file");
                exit(1);
                break;
        }
    }


    if (optind + 1 == argc) {
        input_filename = argv[optind];
    } else {
        fprintf(stderr, "Error: NO input file specified\n");
        exit(1);
    }
    if (output_filename == NULL) {
        fprintf(stderr, "Error: NO output file specified\n");
        exit(1);
    }


    PPMImage * ppm = NULL;
    PBMImage * pbm = NULL;
    PGMImage * pgm = NULL;
    PPMImage * ppm_transformed = NULL;

    ppm = read_ppmfile(input_filename);
    switch (cvt_f) {
        case 'b':
            pbm = convert_ppm_to_pbm(ppm);
            write_pbmfile(pbm, output_filename);
            del_pbmimage(pbm);
            break;
        case 'g':
            pgm = convert_ppm_to_pgm(ppm, ps_factor);
            write_pgmfile(pgm, output_filename);
            del_pgmimage(pgm);
            break;
        case 'i':
            isolate_RGB_channel_inplace(ppm, channel);
            write_ppmfile(ppm, output_filename);
            break;
        case 'r':
            remove_RGB_channel_inplace(ppm, channel);
            write_ppmfile(ppm, output_filename);
            break;
        case 's':
            sepia_transform_inplace(ppm);
            write_ppmfile(ppm, output_filename);
            break;
        case 'm':
            mirror_vertically_inplace(ppm);
            write_ppmfile(ppm, output_filename);
            break;
        case 't':
            ppm_transformed = thumnail_ppm(ppm, ps_factor);
            write_ppmfile(ppm_transformed, output_filename);
            del_ppmimage(ppm_transformed);
            break;
        case 'n':
            nup_ppm_inplace(ppm, ps_factor);
            write_ppmfile(ppm, output_filename);
            break;
        case '?':
            break;
    }

    del_ppmimage(ppm);
    return 0;
    
}
