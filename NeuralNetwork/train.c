#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"

int main()
{
    struct Network n;
    createNetwork(&n, 9, 100, 40);
}
