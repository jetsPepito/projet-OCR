#include "sdl.h"


void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}


/*Save the image using a path */
void SaveImg(const char *path, SDL_Surface *img){
	SDL_SaveBMP(img, path);
	if(!img)
		errx(1, "Couldn't save the image ");
	/*if the image cannot be save return an error*/
}


/*Copy the image in a new image*/
SDL_Surface* CopyImg(SDL_Surface *img){
	int h = img-> h;
	int w = img -> w;
	Uint32 currentpixel; 
	SDL_Surface *copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
			w ,
			h,
			img->format->BitsPerPixel,0,0,0,0);
	for (int i = 0 ; i< w ; i++){
		for (int j = 0 ; j< h; j++){
			currentpixel = getpixel(img, i, j);
			putpixel(copy, i, j, currentpixel);
		}
	}
	return copy;
}


