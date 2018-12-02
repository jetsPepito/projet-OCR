#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

int main()
{
    /* STRUCTURE DU DATASET :
    *  ./dataset_print/arial_2/char.png
    *  char : [0-51] letter corresponding to the char in the array in network
    */

    //Training
    train(10000);

    //Evaluation
    double success_rate = 0;
    for (int j = 0; j < 9; j++)
    {
        //adapt the character
        char i;
        if(j == 0) {i = 'L';}
		else if(j == 1) {i = 'e';}
		else if(j == 2) {i = 'i';}
		else if(j == 3) {i = 'm';}
		else if(j == 4) {i = 'o';}
		else if(j == 5) {i = 'p';}
		else if(j == 6) {i = 'r';}
		else if(j == 7) {i = 's';}
		else {i = 'u';}

        //Create the path
        char *PATH;
        asprintf(&PATH, "./dataset_print/arial/%i.bmp", j);

        //Load the image
        SDL_Surface *img;
        img = IMG_Load(PATH);

        //Call the neural network
        printf("Test %i:\n", j);
        char id = network(img);

        //Check for success rate
        if(id == (char)i) {
            success_rate++;
        }

        //Print the results
        printf("\texpected %c, got %c\n", i, id);

        SDL_FreeSurface(img);
        free(PATH);
    }

    //Print success rate
    printf("Success rate : %g\n", (success_rate / 9));

    return 0;
}
