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
   for (int i = 0 ; i< h ; i++){
     for (int j = 0 ; j< w; j++){

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

void resize(SDL_Surface*img){
  Uint8 r, g, b ;
  float h = img-> h;
  float w = img -> w;
  Uint32 currentpixel;
  int resizetab[784];
  int x, y;
  float hrate = h/ (float)28;
  float wrate = w/ (float)28;
  for(int row = 0; row<28; row++){
    for(int col = 0; col<28; col++){
      x = (row * hrate)+0.5;
      y = (col * wrate)+ 0.5;
      currentpixel= getpixel(img,y,x);
      SDL_GetRGB(currentpixel, img -> format, &r, &g, &b);
      resizetab[row*2 + col] = r ;
      printf("%d", r);
    }
    printf("\n");
  }
}



/*
void create_filematrix(SDL_Surface*img, char *filename){
  Uint8 r, g, b;
  fb = fopen(filename, 'w');
  int h = img -> h;
  int w = img -> w;
  Uint32 currentpixel;
  for (int i = 0 ; i< h ; i++){
    for (int j = 0 ; j< w; j++){
      currentpixel = getpixel(img,i,j);
      SDL_GetRGB(currentpixel,img -> format , &r,&g,&b);

      if(r>= 127 && g>= 127 && b>= 127){
        fputs('1', fb);
      }
      else{
         fputs('0',fb);
      }
    }
    fputs('\n', fb);
  }
  fclose(fb);
}
*/

void SaveImg(char path, SDL_Surface *img){
  int IsImgSave = SDL_SaveBMP(img, path);
  if(IsImgSave != 0)
      printf("Error");
}

SDL_Surface ResizeChar(SDL_Surface *imgchar){
  SDL_Surface *resize_char = SDL_CreateRGBSurface(SDL_HWSURFACE,
                      28,
                      28,
                      imgchar->format->BitsPerPixel,255,255,255,0);
SDL_SoftStretch(imgchar, NULL, &resize_char, NULL);
return *imgchar;
}
