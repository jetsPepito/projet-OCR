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

void init_sdl()
{
	SDL_Init(SDL_INIT_VIDEO);
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


/*Save the image using a path */
void SaveImg(const char *path, SDL_Surface *img){
	SDL_SaveBMP(img, path);
	if(!img)
		errx(1, "Couldn't save the image ");
	/*if the image cannot be save return an error*/
}


SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
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


void update_surface(SDL_Surface *screen, SDL_Surface *img)
{
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, img->w, img->h);
}

