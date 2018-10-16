#ifndef IMAGE_TREATMENT_H
#define IMAGE_TREATMENT_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFunctions/basic.h"
#include <err.h>

void blackwhite(SDL_Surface*img);

void grayscale(SDL_Surface*img);

SDL_Surface* ResizeChar(SDL_Surface *imgchar);

void SaveImg(const char *path, SDL_Surface* img);

#endif
