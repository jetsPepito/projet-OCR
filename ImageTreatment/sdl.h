#ifndef SDL_H_
#define SDL_H_

# include <stdlib.h>
# include <err.h>

# include "../BasicFunctions/basic.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

void wait_for_keypressed(void);

void SaveImg(const char *path, SDL_Surface *img);

SDL_Surface* CopyImg(SDL_Surface *img);

#endif
