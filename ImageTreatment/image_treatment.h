#ifndef IMAGE_TREATMENT_H
#define IMAGE_TREATMENT_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFunctions/basic.h"

void create_filematrix(SDL_Surface*img, char *filename);

void blackwhite(SDL_Surface*img);

void grayscale(SDL_Surface*img);

SDL_Surface* display_image(SDL_Surface *img);

SDL_Surface* copy_image(SDL_Surface *img);

void resize(SDL_Surface *img);

SDL_Surface LoadImg(char path);

SDL_Surface ResizeChar(SDL_Surface *imgchar);

void SaveImg(char path, SDL_Surface* img);

#endif
