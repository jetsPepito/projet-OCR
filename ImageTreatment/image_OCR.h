#ifndef IMAGE_OCR_H
#define IMAGE_OCR_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFonction/basic.h"

void create_filematrix(SDL_Surface*img, char *filename);

void blackwhite(SDL_Surface*img);

void grayscale(SDL_Surface*img);

SDL_Surface* display_image(SDL_Surface *img);

SDL_Surface* copy_image(SDL_Surface *img);
#endif
