#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include "image_treatment.h"
#include "sdl.h"


int main()
{
	//Create everything to test SDL fonction
	SDL_Surface *screen  = NULL, *img = NULL;
	init_sdl();

	//test of grayscale and medianfilter
	img = IMG_Load("images/texte.jpg");
	screen = display_image(img);
	wait_for_keypressed();
	grayscale(img);
	blackwhite(img);
	update_surface(screen,img);
		
	wait_for_keypressed();
	SaveImg("traitement.bmp" ,img );

	//quit
	SDL_Flip(screen);
	SDL_FreeSurface(img);
	SDL_Quit();
	return 0;
}
