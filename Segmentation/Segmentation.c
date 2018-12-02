#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFunctions/basic.h"
#include "../ImageTreatment/image_treatment.h"
#include "Segmentation.h"
#include "Histogram.h"
#include <err.h>
#include "../ImageTreatment/sdl.h"
#include "../NeuralNetwork/network.h"


char* Segmentation (SDL_Surface* img)
{
	int height = img -> h;
	int width = img -> w;
	Uint32 currentPixel;
	int lineInf;
	int lineSup;
	int colInf;
	int colSup;
	int isPixel = 0;
    //int startLine = 0;

    int blank = 0;
    char* text = "";

	Uint8 r,g,b;
    SDL_Surface* character;


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			currentPixel = getpixel(img, j ,i);
			SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

			if (r ==0)
				isPixel = 1;	
		}

		if (isPixel == 1)
		{
			lineInf = i;
			while (i < height && isPixel == 1)
			{
				isPixel = 0;
				for (int j = 0; j < width; j++)
				{
					currentPixel = getpixel(img, j, i);
					SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
					if (r == 0)
						isPixel = 1;
				}
				i++;
			}
			lineSup = i;


			for (int c = 0; c < width; c++)
			{
				for (int l = lineInf; l < lineSup; l++)
				{
					currentPixel = getpixel(img, c, l);
					SDL_GetRGB(currentPixel, img -> format, &r, &g , &b);
					if (r == 0)
						isPixel = 1;
				}
				if (isPixel == 1)
				{
					colInf = c;

                        if (blank >= 10)
                        {
                            asprintf(&text, "%s    ", text);
                            
                        }
                        else
                        {
                            if (blank >= 3)
                                asprintf(&text, "%s ", text);
                        }
                        blank = 0;
                    

					while(c < width && isPixel == 1)
					{
						isPixel = 0;
						for (int l = lineInf; l < lineSup; l++)
						{
							currentPixel = getpixel(img, c, l);
							SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
							if (r == 0)
								isPixel = 1;
						}
						c++;
					}

					colSup = c;

                    // make operation on character here
                    
                    character = SDL_CreateRGBSurface(0, colSup-colInf, lineSup-lineInf, 32, 0, 0, 0, 0);
                    SDL_Rect rectangle = {colInf, lineInf, colSup-colInf , lineSup-lineInf};
                    SDL_Rect* R = &rectangle;

                    SDL_BlitSurface(img, R, character, NULL);
                    character = Cut_Borders(character);
                    character = ResizeChar(character);

                    struct Network n;
                    char txtofchar = analyze(character, &n);
                    //char txtofchar = 'a';
                    asprintf(&text, "%s%c", text, txtofchar);
				}
                else
                {
                    blank++;
                }
			}	
		}
	}
    asprintf(&text, "%s\n", text);
	return text;
}

// remove blank border on the image if they exist

SDL_Surface* Cut_Borders(SDL_Surface *img)
{
    SDL_Surface* cutedImg;
    SDL_Rect rectangle = {0, 0, img -> w, img -> h};
    SDL_Rect* r = &rectangle;

    int height = img -> h;
    int histoX[height];

    int width = img -> w;
    int histoY[width];

    Make_Histogram (img, r, histoX, 0); // horizontal
    Make_Histogram (img, r, histoY, 1); // vertical

    // gather 4 mark to recreate a reduced rectangle
    int i = 0;
    int markSupY = 0;

    while (i < height && histoX[i] == 0)
    {
        markSupY++;
        i++;
    }
    markSupY--;                         // -1 else mark is not the good value due to the while 

    i = height - 1;
    int markInfY = height - 1;
    
    while (i >= 0 && histoX[i] == 0)
    {
        markInfY--;
        i--;
    }
    markInfY++;                         // +1 else mark is not the good value du to the while 
    
    i = 0;
    int markLeftX = 0;

    while (i < width && histoY[i] == 0)
    {
        markLeftX++;
        i++;
    }
    markLeftX--;

    i =  width - 1;
    int markRightX = width - 1;

    while (i >= 0 && histoY[i] == 0)
    {
        markRightX--;
        i--;
    }
    markRightX++;


    rectangle.x = markLeftX;
    rectangle.y = markSupY;
    rectangle.w = markRightX - markLeftX;
    rectangle.h = markInfY - markSupY;

    cutedImg = SDL_CreateRGBSurface(0, rectangle.w, rectangle.h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(img, r, cutedImg, NULL);

    return cutedImg;
}

void SaveSegChar (SDL_Surface* img, char* path)
{
    int height = img -> h;
    int width = img -> w;
    Uint32 currentPixel;
    int lineInf;
    int lineSup;
    int colInf; 
    int colSup;
    int isPixel = 0;
    int iter = 0;
    //char* samepath = path;              // with Append(path, image) path will be change -> segfault my love

    Uint8 r,g,b;
    SDL_Surface* character;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            currentPixel = getpixel(img, j, i);
            SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

            if (r == 0)
                isPixel = 1;
        }

        if (isPixel == 1)
        {
            lineInf = i;
            while (i < height && isPixel == 1)
            {
                isPixel = 0;
                for (int j = 0; j < width; j++)
                {
                    currentPixel = getpixel(img, j, i);
                    SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
                    if (r == 0)
                        isPixel = 1;
                }
                i++;
            }
            lineSup = i;

            for (int c = 0; c < width; c++)
            {
                for (int l = lineInf; l < lineSup; l++)
                {
                    currentPixel = getpixel(img, c, l);
                    SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
                    if (r == 0)
                        isPixel =1;
                }
                if (isPixel == 1)
                {
                    colInf = c;

                    while (c < width && isPixel == 1)
                    {
                        isPixel = 0; 
                        for (int l = lineInf; l < lineSup; l++)
                        {
                            currentPixel = getpixel(img, c, l);
                            SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
                            if (r == 0)
                                isPixel = 1;
                        }
                        c++;
                    }
                    colSup = c;

                    character = SDL_CreateRGBSurface(0, colSup-colInf, lineSup - lineInf, 32, 0, 0, 0, 0);
                    SDL_Rect rectangle = {colInf, lineInf, colSup-colInf, lineSup-lineInf};
                    SDL_Rect* R = &rectangle;
                    
                    SDL_BlitSurface(img, R, character, NULL);
                    character = Cut_Borders(character);
                    character = ResizeChar(character);

                    char* iterator;
                    asprintf(&iterator, "%d", iter);
                    asprintf(&iterator, "%s.bmp", iterator);
                    asprintf(&iterator, "%s%s", path, iterator);
                    printf("%s\n", iterator);

                    iter++;
                    //samepath = path;        //reinit samepath to path else segfault

                    SaveImg(iterator, character);
                }
            }
        }
    }
}
