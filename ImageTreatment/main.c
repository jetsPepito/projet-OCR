#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include "image_treatment.h"
#include "sdl.h"


int main()
{
	//Create everything to test SDL fonction
	SDL_Surface *screen  = NULL, *img = NULL, *grayscal= NULL;
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(1600, 800, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Image", NULL);

	//test of grayscale and medianfilter
	img = IMG_Load("images/texte.jpg");
	SDL_BlitSurface(img, NULL, screen, NULL);
	grayscale(img);
	blackwhite(img);
	SaveImg("grayscale.bmp", img);
	wait_for_keypressed();
	grayscal= IMG_Load("grayscale.bmp");
	medianfilter(grayscal);
	SaveImg("grayscalesansbruit.bmp" ,grayscal );

	//quit
	SDL_Flip(screen);
	wait_for_keypressed();
	SDL_FreeSurface(img);
	SDL_Quit();
	return 0;
}
