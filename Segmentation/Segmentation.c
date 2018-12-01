#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFunctions/basic.h"
#include "../ImageTreatment/image_treatment.h"
#include "Segmentation.h"
#include "Histogram.h"
#include <err.h>
#include "../ImageTreatment/sdl.h"


SDL_Surface* Segmentation (SDL_Surface* img)
{
	int height = img -> h;
	int width = img -> w;
	Uint32 currentPixel;
	int lineInf;
	int lineSup;
	int colInf;
	int colSup;
	int isPixel = 0;

	Uint8 r,g,b;
    SDL_Surface* character;


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			currentPixel = getpixel(img, j ,i);
			SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

			if (r ==0)
			{
				isPixel = 1;	
			}
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
					{
						isPixel = 1;
					}
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
					{
						isPixel = 1;
					}
				}
				if (isPixel == 1)
				{
					colInf = c;
                    // make rectangle and histogram with argument 0

					while(c < width && isPixel == 1)
					{
						isPixel = 0;
						for (int l = lineInf; l < lineSup; l++)
						{
							currentPixel = getpixel(img, c, l);
							SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
							if (r == 0)
							{
								isPixel = 1;
							}
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

					for (int line = lineInf; line < lineSup; line++)
					{
						for( int column = colInf; column < colSup; column++)
						{
							currentPixel = getpixel(img, column, line);
							SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
							r = 100;
							currentPixel = SDL_MapRGB(img -> format, r, g, b);
							putpixel(img, column, line, currentPixel);
						}
					}
				}
			}	
		}
	}
	return character;
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

char* Append(char *str1, char *str2)
{
    size_t size1 = strlen(str1);
    size_t size = size1 + strlen(str2) + 1;

    char* str = realloc(str1, size* sizeof(char));
    if (str == NULL)
        errx(1, "Not enough memory!");

    char *p = str + size1;
    while (*str2 != 0)
        *(p++) = *(str2++);

    *p = 0;

    return str;
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
    char* png = ".png";
    char* samepath = path;              // with Append(path, image) path will be change -> segfault my love

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

                    int length = snprintf(NULL, 0, "%d", iter);
                    char * iterator = malloc(length + 1);

                    snprintf(iterator, length + 1, "%d", iter);
                    Append(iterator, png);
                    //printf("%s\n", samepath);
                    Append(samepath, Append(iterator, png));
                    printf("%s\n", iterator);

                    iter++;
                    samepath = path;        //reinit samepath to path else segfault

                    //SaveImg(iterator, character);
                }
            }
        }
    }
}
