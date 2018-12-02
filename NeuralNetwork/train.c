#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

int main()
{
    struct Network n;
    createNetwork(&n, 52, 100, 71);

	calcSuccess(&n);

    SDL_Surface *img;
    img = SDL_LoadBMP("./dataset_print/arial_2/23.bmp");

    char c = network(img, &n);
    printf("%c\n", c);
}
