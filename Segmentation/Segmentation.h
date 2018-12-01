#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL/SDL.h>

SDL_Surface* Segmentation (SDL_Surface* img);
SDL_Surface* Cut_Borders(SDL_Surface* img);
char* Append(char* str1, char* str2);
void SaveSegChar (SDL_Surface* img, char* path);
#endif
