#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

#define NBTR 10//00000		//NBTRAININGTURNS

int main()
{
    /* STRUCTURE DU DATASET :
    *  ./dataset/char/img.png
    *  char : [0-62] letter corresponding to the char in the array in network
    *  img : [0-100] sample of the letter
    */

    char *PATH = "./dataset/img.png";

    for (int n = 1; n <= NBTR; n++) {
        SDL_Surface *img;
        img = IMG_Load(PATH); //charger une image aleatoire
        char expected = 'M'; //caractere correspondant
        char id = network(img, 't', 'y', expected);

        printf("Round %i: expected %c, got %c\n", n, expected, id);
    }

    return 0;
}
