#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "../BasicFunctions/basic.h"
#include "Segmentation.h"


SDL_Surface* CutLine (SDL_Surface*img)

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

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			currentPixel = getpixel(img, j ,i);
			SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

			if (r !=255 && g !=255 && b !=255)
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
					if (r != 255 && g !=255 && b != 255)
					{
						isPixel = 1;
					}
				}
				i++;
			}
			lineSup = i; // no need de decreament
			

			for (int c = 0; c < width; c++)
			{
				for (int l = lineInf; l < lineSup; l++)
				{
					currentPixel = getpixel(img, c, l);
					SDL_GetRGB(currentPixel, img -> format, &r, &g , &b);
					if (r !=255 && g !=255 && b !=255)
					{
						isPixel = 1;
					}
				}
				if (isPixel == 1)
				{
					colInf = c;

					while(c < width && isPixel == 1)
					{
						isPixel = 0;
						for (int l = lineInf; l < lineSup; l++)
						{
							currentPixel = getpixel(img, c, l);
							SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
							if (r !=255 && g !=255 && b !=255)
							{
								isPixel = 1;
							}
						}
						c++;
					}
					colSup = c;   // no need to decreament

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
	return img;
}
