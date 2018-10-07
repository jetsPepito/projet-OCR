# include <stdlib.h>
# include <SDL/SDL.h>

void putpixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
Uint32 getpixel(SDL_Surface *surface, unsigned x, unsigned y);
static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y);
