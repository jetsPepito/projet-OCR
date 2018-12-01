#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "Segmentation.h"
#include "../ImageTreatment/image_treatment.h"

void pause()
{
	int continuer = 1;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
		}
	}
}

int main()
{
    SDL_Surface *ecran = NULL, *image = NULL;
	SDL_Init(SDL_INIT_VIDEO);
    //char* path = "../NeuralNetwork/dataset_print/arial_2/";
    char* path = "";

	ecran = SDL_SetVideoMode(1600, 800, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Chargement d'images en SDL", NULL);
	//image = SDL_LoadBMP("Lorem-Ipsum.bmp");
	//image = SDL_LoadBMP("ascii.bmp");
    image = IMG_Load("../NeuralNetwork/dataset_print/arial_2/dataset.png");
    blackwhite(image);
    
	SDL_Surface *segmentedImg;
	//segmentedImg = Segmentation(image);
    //segmentedImg = Cut_Borders(image);
    SaveSegChar (image, path);
    //char* str1 = "Hello";
    //char* str2 = "World";
    //char* fulltext = Append(str1, str2);
    //printf("%s\n", fulltext);
	SDL_BlitSurface(segmentedImg, NULL, ecran, NULL);
	SDL_Flip(ecran);
	pause();

	SDL_FreeSurface(image);
	SDL_Quit();
	return 0;
}
