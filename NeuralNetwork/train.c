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
    network(NULL, 't');

    double success_rate = 0;
    //Post-training test
    for (int j = 0; j < 52; j++) {
        //adapt the character
        char i;
        if(j >= 0 && j <= 25) {i = j + 65;} //uppercase
        else {i = j + 71;} //lowercase

        //Create the path
        char *PATH;
        asprintf(&PATH, "./dataset_print/arial_2/%i.bmp", j);
        //Load the image
        SDL_Surface *img;
        img = IMG_Load(PATH);
        //Call the neural network
        char id = network(img, 'e');
        if(id == (char)i) {
            success_rate++;
        }
        //Print the results
        id += 1 - 1; //so it compiles
        printf("Test %i: expected %c, got %c\n", j, i, id);
        free(img);
        free(PATH);
    }
    printf("Success rate : %g\n", (success_rate / 58));

    return 0;
}
