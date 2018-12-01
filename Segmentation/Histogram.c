#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "Type.h"
#include "Histogram.h"
#include "../BasicFunctions/basic.h"

//count all black pixels on every line of image
int Make_Histogram(SDL_Surface *img, SDL_Rect* rectangle, int histogram[], int axis)
    {
    int lineMax = rectangle -> h;
    int colMax = rectangle -> w;
    int lineMin = rectangle -> x;
    int colMin = rectangle -> y;

    Uint32 currentPixel;
    Uint8 r, g, b;

    if (axis == 0)
    {
        
        for (int i = lineMin; i <= lineMax; i++)
        {
            
            histogram[i-lineMin] = 0;
            for (int j = colMin; j < colMax; j++)
            {
                currentPixel = getpixel(img, j, i);
                SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

                if (r == 0)
                {
                    histogram[i-lineMin]++;
                }
            }
        }
    }
    else
    {
        for (int j = colMin; j <= colMax; j++)
        {
            histogram[j-colMin] = 0;
            for (int i = colMin; i < lineMax; i++)
            {
                currentPixel = getpixel(img, j, i);
                SDL_GetRGB(currentPixel, img -> format, &r, &g,& b);

                if (r == 0)
                {
                    histogram[j-colMin]++;
                }
            }
        }
    }
    return 0;
}
