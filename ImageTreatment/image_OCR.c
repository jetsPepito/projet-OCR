 #include "image_OCR.h"



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
  for (int i = 0 ; i< h ; i++){
    for (int j = 0 ; j< w; j++){

      currentpixel = getpixel(img,i,j);
      SDL_GetRGB(currentpixel,img -> format, &r,&g,&b);
      gray = (int) (r * 0.2126 + g* 0.7152 + b* 0.0722);
      currentpixel = SDL_MapRGB(img->format, gray, gray, gray);
      putpixel(img, i, j , currentpixel);
    }
  }
}

void create_filematrix(SDL_Surface*img, char *filename){
  Uint8 r, g, b;
  fb = fopen(filename, 'w');    /* crée un fichier du nom filename PAS DE GESTION DU .txt POUR LE MOMENT*/
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

static inline Uint8* pixelref(SDL_Surface *surf, unsigned x, unsigned y)
{
 int bpp = surf -> format -> BytesPerPixel;
 return (Uint8*)surf -> pixels + y * surf -> pitch + x * bpp;
}

SDL_Surface* LoadImg(char path){
    SDL_Surface *img;
    img = IMG_Load(path);
    if (!img){
        printf("ERREUR")  /*REMPLACER PAR UNE VRAI ERREUR PLUS TARD */
    }
    return img
}



SDL_Surface* resize(SDL_Surface *img){
    SDL_Surface *res
