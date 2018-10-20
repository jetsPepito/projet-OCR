#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "Type.h"

//count all pixels on every line of image
DoubleInt Make_Histogram(SDL_Surface *img, Rectangle rectangle,int axis)
{
    int lineMax = rectangle.BotRight.l;
    int colMax = rectangle.BotRight.c;
    int lineMin = rectangle.TopLeft.l;
    int colMin = rectangle.Topleft.c;

    Uint32 currentPixel;
    Uint8, r, g, b;

    DoubleInt list&length;

    if (axis = 0)
    {
        int list&length.c = (lineMax - lineMin)+1;
        int list&length.l[list&length.c];
        for (int i = lineMin; i <= lineMax; i++)
        {
            list&length.l[i-lineMin] = 0;
            for(int j = colMin; j <= lineMax j++)
            {
                currentPixel = getpixel(img, j, i);
                SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

                if (r ==0 && g==0 && b==0)
                {   
                    list&length.l[i-lineMin]++;
                }
            }
        }
    }
    else
    {
        int list&length.c = (colMin - colMax)+1;
        int list&length.l[list&length.c];
        for (int j = colMin; i <= colMax; j++)
        {
            list&length.l[j-colMin] = 0;
            for (int i=0; i <= lineMax; j++)
            {
                currentPixel = getpixel(img, j, i);
                SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);

                if (r==0 && g==0 && b==0)
                {
                    histogram.l[j-colMin]++;
                }
            }
        }
    }
    return list&length;
}

DoubleRect Divide_Image(SDL_Surface *img, Rectangle rectangle,  int i, int axis)
{
    Uint32 currentPixel;
    Uint8 r,g,b;

    Rectangle divImg1;
    Rectangle divImg2;

    doubleRect dividedImage;

    if (axis == 0)		//horizontal division
    {
        divImg1.TopLeft = rectangle.TopLeft;
        divImg1.TopRight = rectangle.TopRight;
        divImg1.BotLeft.l = i-1;
        divImg1.BotLeft.c = rectangle.BotLeft.c;
        divImg1.BotRight.l = i-1;
        divImg1.BotRight.c = rectangle.BotRight.c;

        divImg2.TopLeft.l = i+1;
        divImg2.TopLeft.c = rectangle.TopLeft.c;
        divImg2.TopRight.l = i+1;
        divImg2.TopRight.c = rectangle.TopRight.c;
        divImg2.BotLeft = rectangle.BotLeft;
        divImg2.BotRight = rectangle.BotRight;

        for (int j=0; j <= rectangle.TopRight.c; j++)
        {
            currentPixel = getpixel(img, j, i);
            SDL_GetRGB(currentPixel, img -> format, &r, &g, &b);
            r = 100;
            currentPixel = SDL_MapRGB(img -> format, r, g, b);
            putpixel(img, j, i, currentPixel);
        }
    }
    else //vertical division
    {
        divImg1.TopLeft = rectangle.TopLeft;
        divImg1.TopRight.l = rectangle.TopRight.l;
        divImg1.TopRight.c = i-1;
        divImg1.BotLeft = rectangle.BotLeft;
        divImg1.BotRight.l = rectangle.BotRight.l;
        divImg1.BotRight.c = i-1;

        divImg2.TopLeft.l = rectangle.TopLeft.l;
        divImg2.TopLeft.c = i+1;
        divImg2.TopRight = rectangle.TopRight;
        divImg2.BotLeft.l = rectangle.BotLeft.l;
        divImg2.BotLeft.c = i+1;
        divImg2.BotRight = rectangle.BotRight;

        for(int j=0; j <= rectangle.TopRight.l; j++)
        {
            currentPixel = getpixel(img, i, j);
            SDL_GetRGB(currentPixel, img -> format, &r, &g, &b)
                r = 100;
            currentPixel = SDL_MapRGB(img -> format, r, g, b);
            putpixel(img, i, j, currentPixel);
        }
    }

    dividedImage.Rect1 = divImg1;
    dividedImage.Rect2 = divImg2;

    return dividedImage; 
}

int Checklist(DoubleInt list&length)
{
    int isChar = 0;
    int i = 0;
    
    while (i < list&length.c && is Char = 0)
    {
        if (list&length.l[i] == 0)
        {
            isChar = 1;
        }
    }
    return isChar;
}

int Find_Index(DoubleInt list&length)
{
    int markLow = NULL;
    int markUp = NULL;
    int markLowTemp = NULL;
    int markUpTemp = NULL;

    for (int i =0; i < list&length.c; i++)
    {
        if (list&length.l[i] == 0)
        {
            markLowTemp = i;
            markUpTemp = i;

            while (i < list&length.c && list&length.l[i] == 0)
            {
                markUpTemp = i;
            }
            
            if (markUpTemp - markLowTemp > markUp  - marLow)
        }
    }
}

void RXY_Cut(SDL_Surface *img, Rectangle rectangle,  int axis)
{
    DoubleInt list&length =  Make_Histogram(img, rectangle, axis);

    
}
