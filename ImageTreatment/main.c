#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include "image_treatment.h"


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
	SDL_Surface *ecran = NULL, *imageDeFond = NULL;

	SDL_Init(SDL_INIT_VIDEO);

	ecran = SDL_SetVideoMode(1600, 800, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Image", NULL);

	imageDeFond = IMG_Load("images/aigle.bmp");
	SDL_BlitSurface(imageDeFond, NULL, ecran, NULL);
	SDL_Surface *resizeimg;
	SDL_Surface *copy;
	copy = CopyImg(imageDeFond);
	resizeimg =ResizeChar(imageDeFond);
	grayscale(imageDeFond);
	SaveImg("blacknwhite.jpg", imageDeFond);
	SaveImg(" neawinmage.bmp", resizeimg);
	SaveImg("copyimg.jpg", copy);
	SDL_Flip(ecran);
	pause();

	SDL_FreeSurface(imageDeFond);
	SDL_Quit();
	return 0;
}
