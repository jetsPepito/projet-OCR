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
    SDL_WM_SetCaption("Chargement d'images en SDL", NULL);

    imageDeFond = SDL_LoadBMP("images/aigle.bmp");
    SDL_BlitSurface(imageDeFond, NULL, ecran, NULL);
    ResizeChar(imageDeFond);
    SDL_SaveBMP(imageDeFond," neawinmage.bmp");
    SDL_Flip(ecran);
    pause();

    SDL_FreeSurface(imageDeFond);
    SDL_Quit();
    return 0;
}
