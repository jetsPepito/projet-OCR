#ifndef NETWORK_H
#define NETWORK_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/* Params:
    -*src: the image of the character, 28*28 pixels
    -mode: 't' for training, anything else for evaluation (USE 'e')
    -reset: 'y' for yes (clears the saves), anything else for no (USE 'n')
    -expected: the expected character for training (USE '0')
   Returns:
    -type char: the identified character from the image
   DO NOT RESET THE WEIGHTS IN ANY CIRCUMSTANCE !
*/
char network(SDL_Surface *src, char mode, char reset, char expected);

#endif
