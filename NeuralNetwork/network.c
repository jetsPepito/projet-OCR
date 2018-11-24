#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 785		//NBINPUTS			i : 784 + bias	| 28*28 px image
#define NBHN 784		//NBHIDDENNET		h1: 784
#define NBHO 785		//NBHIDDENOUT		h2: 784 + bias
#define NBOU 63			//NBOUTPUTS			o : 62  + bias	| lettersUL + digits
#define NBTR 100000		//NBTRAININGTURNS
#define ETA 0.1			//LEARNING RATE
#define PATH "nn.txt"	//SAVE FILE FOR THE WEIGTHS

/*============================================================================*/



/*======================= Define weight  data structure ======================*/

typedef struct weights
{
	double *wInpHid;
	double *wHidOut;
}weights;

weight *newW()
{
	weight *w = (weight *) malloc(sizeof(weight));

	w->wInpHid = (double *) malloc(sizeof(double) * (NBIN * NBHN));
	w->wHidOut = (double *) malloc(sizeof(double) * (NBHO * NBOU));

	return w;
}

void freeW(weight *w)
{
	free (w->wInpHid)
	free (w->wHidOut)
	free (w)
}

/*============================================================================*/



/*============================= Declare functions ============================*/

//sigmoid
double sig(double x);
//sigmoid prime
double sigp(double x);
//softmax
double softmax();
//softmax prime
double softmaxp();
//save weigths
void save(double *wIH, double *wHO);
//load weigths
void load(double *wIH, double *wHO);
//initialize pointers
void init(char reset, SDL_surface *src, double *inputs, double *wIH, \
double *hOut, double *wHO);
//forward and backward propagation
void identify(char mode, double *inputs, double *wIH, double *hNet, \
double *hOut, double *wHO, double *net, double *out, char expected);
//main function to call | mode = (t)rain/(e)val | reset weigths = (y)es/(n)o
char network(SDL_surface *src, char mode, char reset, char expected);

/*============================================================================*/



/*=================================== Code ===================================*/

//sigmoid
double sig(double x)
{
	return (1 / (1 + exp(-x)));
}

//sigmoid prime
double sigp(double x)
{
	double sigx = sig(x);
	return (sigx * (1 - sigx));
}

//softmax
double softmax()
{
	// TODO
	return 0.0
}

//softmax prime
double softmaxp()
{
	// TODO
	return 0.0
}

//save weigths
void save(double *wIH, double *wHO)
{
	weight *w = newW();

	*w->wInpHid = *wIH;
	*w->wHidOut = *wHO;

	FILE *f = fopen(PATH);
	fwrite(w, sizeof(weight), 1, f)
}

//load weigths
void load(double *wIH, double *wHO)
{
	weight *w = newW();
	FILE *f = fopen(PATH);
	fread(w, sizeof(weight), 1, f)

	*wIH = *w->wInpHid;
	*wHO = *w->wHidOut;
}

//initialize pointers
void init(char reset, SDL_surface *src, double *inputs, double *wIH, \
double *hOut, double *wHO)
{
	/*Weights*/
	if(reset == 'n') {
		load(wIH, wHO);
	} else {
		for(int i = 0; i < NBIN; i++) {
			for(int h1 = 1; h1 < NBHN; h1++) {
				wIH[i * NBHN + h1] = (double)rand()/RAND_MAX*2.0-1.0;
			}
		}
		for(int h2 = 0; h2 < NBHO; h2++) {
			for(int o = 1; o < NBOU; o++) {
				wHO[h2 * NBOU + o] = (double)rand()/RAND_MAX*2.0-1.0;
			}
		}
	}

	/*Inputs*/
	inputs[0] = 1.0; // bias
	uint32 px;
	uint8 r;
	uint8 g;
	uint8 b;
	for(int x = 0; x < 28; x++) {
		for(int y = 0; y < 28; y++) {
			px = getpixel(src, x, y)
			SDL_GetRGB(px, img->format, &r, &g, &b);
			inputs[x * 28 + y + 1] = (r == 255) ? 0.0 : 1.0 // b = 1, w = 0
		}
	}

	/*Hidden layer*/
	hOut[0] = 1.0; // bias

}

//forward and backward propagation
void identify(char mode, double *inputs, double *wIH, double *hNet, \
double *hOut, double *wHO, double *net, double *out, char expected)
{
	// TODO
	return
}

//main function to call | mode = (t)rain/eval | reset weigths = (n)o/yes
char network(SDL_surface *src, char mode, char reset, char expected)
{
	/*Initialize RNG*/
	time_t t;
	srand((unsigned) time(&t));

	/*Initilize the list of possible chars*/
	char *CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	/*Declare pointers*/
	double *inputs = (double *) malloc(sizeof(double) * NBIN);
	double *wIH = (double *) malloc(sizeof(double) * (NBIN * NBHN));
	double *hNet = (double *) malloc(sizeof(double) * NBHN);
	double *hOut = (double *) malloc(sizeof(double) * NBHO);
	double *wHO = (double *) malloc(sizeof(double) * (NBHO * NBOU));
	double *net = (double *) malloc(sizeof(double) * NBOU);
	double *out = (double *) malloc(sizeof(double) * NBOU);

	/*Initialize pointers*/
	init(reset, src, inputs, wIH, hOut, wHO);

	/*Propagate*/
	identify(mode, inputs, wIH, hNet, hOut, wHO, net, out, expected);

	/*Identify the character*/
	double mostprob = 0.0;
	int result = 0;
	for(int i = 0; i < NBOU; i++) {
		if(out[i] > mostprob) {
			mostprob = out[i];
			result = i;
		}
	}

	/*Save the corrected weights*/
	if(mode == 't') {
		save(wIH, wHO);
	}
	/*Free memory*/
	free(inputs);
	free(wIH);
	free(hNet);
	free(hOut);
	free(wHO);
	free(net);
	free(out);
	free(CHARS);

	return CHARS[i];
}

/*============================================================================*/
