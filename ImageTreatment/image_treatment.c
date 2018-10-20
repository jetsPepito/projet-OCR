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
	SDL_SaveBMP(img, path);
	if(!img)
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



void medianfilter(SDL_Surface *img){
	int h = img-> h;
	int w = img -> w;
	int array[9];
	Uint32 swap;
	Uint32 mid ;
	int j = 0;
	for(int i = 0 ; i<w ; i++){
		for(int k = 0 ; k<h ; k++){
			for(j = k ; j< k+4; j++){
				//Borders
				if(i == 0){
					if(j == 0){
						array[0] = getpixel(img , i , j);
						array[1] = getpixel(img , i, j);
						array[2] = getpixel(img, i+1 , j);
						array[3] = getpixel(img, i , j+1);
						array[4] = getpixel(img, i, j);
						array[5] = getpixel(img , i+1, j+1);
						array[6] = getpixel(img, i , j);
						array[7] = getpixel(img, i , j);
						array[8] = getpixel(img , i ,j);
						break;
					}
					if(j == h){
						array[0] = getpixel(img , i , j-1);
						array[1] = getpixel(img , i, j);
						array[2] = getpixel(img, i+1 , j);
						array[3] = getpixel(img, i , j);
						array[4] = getpixel(img, i, j);
						array[5] = getpixel(img , i+1, j-1);
						array[6] = getpixel(img, i , j);
						array[7] = getpixel(img, i , j);
						array[8] = getpixel(img , i ,j);

						break;
					}
					array[0] = getpixel(img , i , j-1);
					array[1] = getpixel(img , i, j+1);
					array[2] = getpixel(img, i+1 , j);
					array[3] = getpixel(img, i , j);
					array[4] = getpixel(img, i, j);
					array[5] = getpixel(img , i+1, j-1);
					array[6] = getpixel(img, i+1 , j+1);
					array[7] = getpixel(img, i , j);
					array[8] = getpixel(img , i ,j);

					break;
				}
				if(i == w){
					if(j == 0){
						array[0] = getpixel(img , i , j);
						array[1] = getpixel(img , i, j+1);
						array[2] = getpixel(img, i-1 , j);
						array[3] = getpixel(img, i , j);
						array[4] = getpixel(img, i, j);
						array[5] = getpixel(img , i-1, j+1);
						array[6] = getpixel(img, i , j);
						array[7] = getpixel(img, i , j);
						array[8] = getpixel(img , i ,j);

						break;
					}
					if(j == h){
						array[0] = getpixel(img , i , j-1);
						array[1] = getpixel(img , i, j);
						array[2] = getpixel(img, i-1 , j);
						array[3] = getpixel(img, i , j);
						array[4] = getpixel(img, i, j);
						array[5] = getpixel(img , i-1, j-1);
						array[6] = getpixel(img, i , j);
						array[7] = getpixel(img, i , j);
						array[8] = getpixel(img , i ,j);

						break;
					}
					array[0] = getpixel(img , i , j-1);
					array[1] = getpixel(img , i, j+1);
					array[2] = getpixel(img, i-1 , j);
					array[3] = getpixel(img, i , j);
					array[4] = getpixel(img, i, j);
					array[5] = getpixel(img , i-1, j+1);
					array[6] = getpixel(img, i-1 , j-1);
					array[7] = getpixel(img, i , j);
					array[8] = getpixel(img , i ,j);

					break;

				}
				if(j == 0){
					array[0] = getpixel(img, i, j);
					array[1] = getpixel(img, i, j);
					array[2] = getpixel(img, i-1, j);
					array[3] = getpixel(img, i, j+1);
					array[4] = getpixel(img, i+1, j);
					array[5] = getpixel(img , i-1, j+1);
					array[6] = getpixel(img, i+1 , j+1);
					array[7] = getpixel(img, i , j);
					array[8] = getpixel(img , i ,j);

					break;
				}
				if(j == h){
					array[0] = getpixel(img, i, j);
					array[1] = getpixel(img, i, j);
					array[2] = getpixel(img, i-1, j);
					array[3] = getpixel(img, i, j-1);
					array[4] = getpixel(img, i+1, j);
					array[5] = getpixel(img , i+1, j-1);
					array[6] = getpixel(img, i-1 , j-1);
					array[7] = getpixel(img, i , j);
					array[8] = getpixel(img , i ,j);

					break;

				}
				else{
					//general case
					array[0] = getpixel(img, i-1, j);
					array[1] = getpixel(img, i, j);
					array[2] = getpixel(img, i, j-1);
					array[3] = getpixel(img, i, j+1);
					array[4] = getpixel(img, i+1, j);
					array[5] = getpixel(img , i-1, j-1);
					array[6] = getpixel(img, i+1 , j-1);
					array[7] = getpixel(img, i-1 , j+1);
					array[8] = getpixel(img , i+1 ,j+1);

					break;
				}
			}
			/*Bubble sort of the list */
			for (int x = 0; x < 8; x++){
				for (int y = 0; y < 8-x; y++){
					if (array[y] > array[y+1]){
						swap = array[y];
						array[y] = array[y+1];
						array[y+1] = swap;
					}
				}
			}
			mid = array[4];
			putpixel(img, i, j, mid);

		}
	}

}
