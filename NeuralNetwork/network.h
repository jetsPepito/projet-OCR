#ifndef NETWORK_H
#define NETWORK_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/* Params:
    -*src: the image of the character, 28*28 pixels
    -mode: 't' for training, 'e' evaluation (USE 'e')
   Returns:
    -type char: the identified character from the image
*/
char network(SDL_Surface *src, char mode);

#endif
