#include "image_treatment.h"


void blackwhite(SDL_Surface*img){
	/*Variables*/
	Uint8 r;
	Uint8 g;
	Uint8 b;
	int h = img -> h;
	int w = img -> w;
	Uint32 currentpixel;
	/*each pixel will become black or white */
	for (int i = 0 ; i< w ; i++){
		for (int j = 0 ; j< h; j++){
			currentpixel = getpixel(img,i,j);
			SDL_GetRGB(currentpixel,img -> format , &r,&g,&b);

			if(r>= 127 && g>= 127 && b>= 127){
				r = 255;
				g = 255;
				b = 255;
			}
			else{
				r = 0;
				g = 0;
				b = 0;
			}

			currentpixel = SDL_MapRGB(img->format, r, g, b);
			putpixel(img, i, j , currentpixel);
		}
	}
}


void grayscale(SDL_Surface*img){
	/*Variables*/
	Uint8 r, g, b, gray ;
	int h = img-> h;
	int w = img -> w;
	Uint32 currentpixel;
	/*for each pixel we applicate the filter*/
	for (int i = 0 ; i< w ; i++){
		for (int j = 0 ; j< h; j++){

			currentpixel = getpixel(img,i,j);
			SDL_GetRGB(currentpixel,img -> format, &r,&g,&b);
			gray = (int) (r * 0.2126 + g* 0.7152 + b* 0.0722);
			currentpixel = SDL_MapRGB(img->format, gray, gray, gray);
			putpixel(img, i, j , currentpixel);
		}
	}
}


/*Save the image using a path */
void SaveImg(const char *path, SDL_Surface *img){
	int IsImgSave = SDL_SaveBMP(img, path);
	if(IsImgSave != 0)
		errx(1, "Couldn't save the image ");
	/*if the image cannot be save return an error*/
}


/* Resize the SDL surface of the char in 28x28 for the NeuralNetwork*/
SDL_Surface* ResizeChar(SDL_Surface *imgchar){
	SDL_Surface *resize_char = SDL_CreateRGBSurface(SDL_HWSURFACE,
			28,
			28,
			imgchar->format->BitsPerPixel,0,0,0,0);
	SDL_SoftStretch(imgchar, NULL, resize_char, NULL);
	return resize_char;
}

