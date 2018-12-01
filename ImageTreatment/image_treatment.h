#ifndef IMAGE_TREATMENT_H
#define IMAGE_TREATMENT_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include <SDL/SDL_image.h>
#include "../BasicFunctions/basic.h"
#include "../Segmentation/Segmentation.h"

unsigned long medianpixel(SDL_Surface *img);

void blackwhite(SDL_Surface*img);

void grayscale(SDL_Surface*img);

SDL_Surface* ResizeChar(SDL_Surface *imgchar);

char *convert(SDL_Surface *img);

void medianfilter(SDL_Surface *img);

#endif
