#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

#define NBTR 10//00000		//NBTRAININGTURNS

int main()
{
    /* STRUCTURE DU DATASET :
    *  ./dataset_print/police/char.png
    *  police : [1-4] Arial, Calibri, Cambria, Georgia
    *  char : [33-126] letter corresponding to the char in the array in network
    */


    /* FIXING THE FORWARD PROPAGATION (note to self)
    *   -the net arrays never change at the index 0
    *   -check if the arrays have the right size
    *   -I suspect they go one index too far and actualy start at 1...
    *   -do more tests after getting some sleep
    */

    srand(time(NULL));

    for (int n = 1; n <= NBTR; n++) {
        //Select a random police and character
        int pol = rand()%3;
        char expected = (char)((rand()%(126-33))+33);
        //Create the corresponding path
        char *PATH;
        asprintf(&PATH, "./dataset_print/%i/%i.png", pol, expected);
        //Load the image
        SDL_Surface *img;
        img = IMG_Load(PATH);
        //Call the neural network
        char id = network(img, 't', 'y', expected);
        //Print the results
        printf("Round %i: police %i, expected %c, got %c\n", n, pol, expected, id);
        free(img);
        free(PATH);
    }

    return 0;
}
