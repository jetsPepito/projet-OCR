#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

int main()
{
    /* STRUCTURE DU DATASET :
    *  ./dataset_print/arial/char.png
    *  char : [33-122] letter corresponding to the char in the array in network
    */

    //Training
    network(NULL, 't');

    double success_rate = 0;
    //Post-training test
    for (int j = 0; j < 68; j++) {
        //adapt the character
        char i;
        if(j >= 0 && j <= 9) {i = j + 48;} //digits
        else if(j >= 10 && j <= 35) {i = j + 55;} //uppercase
        else if(j >= 36 && j <= 61) {i = j + 61;} //lowercase
        else if(j == 62) {i = 33;} // !
        else if(j == 63) {i = 39;} // '
        else if(j == 64) {i = 44;} // ,
        else if(j == 65) {i = 46;} // .
        else if(j == 66) {i = 58;} // :
        else if(j == 67) {i = 63;} // ?

        //Create the path
        char *PATH;
        asprintf(&PATH, "./dataset_print/arial/%i.png", j);
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
    printf("Success rate : %g\n", (success_rate / 90));

    return 0;
}
