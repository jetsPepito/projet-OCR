#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "Segmentation.h"

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
	SDL_WM_SetCaption("Chargement d'images en SDL", NULL);
	imageDeFond = SDL_LoadBMP("Lorem-Ipsum.bmp");
	//imageDeFond = SDL_LoadBMP("ascii.bmp");
	//imageDeFond = IMG_Load("Alphabet.png");

	SDL_Surface *segmentedImg;
	segmentedImg = CutLine(imageDeFond);
	SDL_BlitSurface(segmentedImg, NULL, ecran, NULL);
	SDL_Flip(ecran);
	pause();

	SDL_FreeSurface(imageDeFond);
	SDL_Quit();
	return 0;
}
