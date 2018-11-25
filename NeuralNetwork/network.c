#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 785		//NBINPUTS			i : 784 + bias	| 28*28 px image
#define NBHN 784		//NBHIDDENNET		h1: 784
#define NBHO 785		//NBHIDDENOUT		h2: 784 + bias
#define NBOU 63			//NBOUTPUTS			o : 62  + bias	| lettersUL + digits
#define ETA 0.1			//LEARNING RATE

/*============================================================================*/



/*======================= Define weights data structure ======================*/

typedef struct weight
{
	double *wInpHid;
	double *wHidOut;
}weight;

weight *newW()
{
	weight *w = (weight *) malloc(sizeof(weight));

	w->wInpHid = (double *) malloc(sizeof(double) * (NBIN * NBHN));
	w->wHidOut = (double *) malloc(sizeof(double) * (NBHO * NBOU));

	return w;
}

void freeW(weight *w)
{
	free (w->wInpHid);
	free (w->wHidOut);
	free (w);
}

/*============================================================================*/



/*============================= Declare functions ============================*/
/*
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
	double *hNet, double *hOut, double *wHO, double *net)
//forward and backward propagation
void identify(char mode, double *inputs, double *wIH, double *hNet, \
	double *hOut, double *wHO, double *net, double *out, char expected);
//main function to call | mode = (t)rain/(e)val | reset weigths = (y)es/(n)o
char network(SDL_surface *src, char mode, char reset, char expected);
*/
/*============================================================================*/



/*============================= Declare functions ============================*/
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
void softmax(double *net, double *out)
{
	double sum = 0;
	for(int k = 0; k < NBOU; k++) {
		sum += exp(net[k]);
	}
	for(int j = 0; j < NBOU; j++) {
		out[j] = exp(net[j]) / sum;
	}
}

//softmax prime
double softmaxp(double *out, int j, int i)
{
	return (out[i] * (((i == j) ? 1 : 0) - out[j]));
}

//save weigths
void save(double *wIH, double *wHO)
{
	char *PATH = "weights";
	weight *w = newW();

	w->wInpHid = wIH;
	w->wHidOut = wHO;

	FILE *f = fopen(PATH, "wb");
	fwrite(w, sizeof(weight), 1, f);

	freeW(w);
}

//load weigths
void load(double *wIH, double *wHO)
{
	char *PATH = "weights";
	weight *w = newW();
	FILE *f = fopen(PATH, "rb");
	fread(w, sizeof(weight), 1, f);

	*wIH = *(w->wInpHid);
	*wHO = *(w->wHidOut);

	freeW(w);
}

//initialize pointers
void init(char reset, SDL_Surface *src, double *inputs, double *wIH, \
	double *hNet, double *hOut, double *wHO, double *net)
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
	Uint32 px;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	for(int x = 0; x < 28; x++) {
		for(int y = 0; y < 28; y++) {
			px = getpixel(src, x, y);
			SDL_GetRGB(px, src->format, &r, &g, &b);
			inputs[x * 28 + y + 1] = (r == 255) ? 0.0 : 1.0; // b = 1, w = 0
		}
	}

	/*Hidden layer*/
	for(int h1 = 0; h1 < NBHN; h1++) {
		hNet[h1] = 0.0;
	}
	hOut[0] = 1.0; // bias

	/*Outputs*/
	for(int o = 0; o < NBOU; o++) {
		net[o] = 0.0;
	}

}

//forward and backward propagation
void identify(char mode, double *inputs, double *wIH, double *hNet, \
	double *hOut, double *wHO, double *net, double *out, char expected)
{
	/*========== Forward Propagation ==========*/
	// inputs -> hidden layer
	for(int i = 0; i < NBIN; i++) {
		for(int h1 = 0; h1 < NBHN; h1++) {
			hNet[h1] += inputs[i] * wIH[i * NBHN + h1];
		}
	}

	// hidden layer activation
	for(int h1 = 0; h1 < NBHN; h1++) {
		hOut[h1+1] = sig(hNet[h1]);
	}

	// hidden layer -> outputs
	for(int h2 = 0; h2 < NBHO; h2++) {
		for(int o = 0; o < NBOU; o++) {
			net[o] += hOut[h2] * wHO[h2 * NBOU + o];
		}
	}

	// outputs activation
	softmax(net, out);
	/*=========================================*/

	/*========== Backward Propagation =========*/
	if(mode == 't') {
		char *CHARS="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		// Output -> Hidden Layer
		// -= dErr/dOut * dOut/dNet * dNet/dWHO * ETA
		for(int h2 = 0; h2 < NBHO; h2++) {
			for(int o = 0; o < NBOU; o++) {
				wHO[h2 * NBOU + o] -= ((out[o] - ((CHARS[o] == expected) ? 1 : 0))
										* softmaxp(out, h2, o)
				 						* hOut[h2] * ETA);
			}
		}

		// Hidden layer -> Inputs
		// -= dErr/dOut * dOut/dNet * dNet/dHOut * dHout/dHNet * dHNet/dWIH  *ETA
		for(int i = 0; i < NBIN; i++) {
			for(int h1 = 0; h1 < NBHN; h1++) {
				double deltaHO = 0.0;
				for(int o = 0; o < NBOU; o++) {
					deltaHO += ((out[o] - ((CHARS[o] == expected) ? 1 : 0))
								* softmaxp(out, h1 + 1, o)
					 			* wHO[(h1 + 1) * NBOU + o]);
				}
				wIH[i * NBHN + h1] -= (deltaHO * sigp(hNet[h1]) * inputs[i] * ETA);
			}
		}
	}
	/*=========================================*/
}

//main function to call | mode = (t)rain/eval | reset weigths = (n)o/yes
char network(SDL_Surface *src, char mode, char reset, char expected)
{
	/*Initialize RNG*/
	time_t t;
	srand((unsigned) time(&t));

	/*Initilize the list of possible chars*/
	char *CHARS="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	/*Declare pointers*/
	double *inputs = (double *) malloc(sizeof(double) * NBIN);
	double *wIH = (double *) malloc(sizeof(double) * (NBIN * NBHN));
	double *hNet = (double *) malloc(sizeof(double) * NBHN);
	double *hOut = (double *) malloc(sizeof(double) * NBHO);
	double *wHO = (double *) malloc(sizeof(double) * (NBHO * NBOU));
	double *net = (double *) malloc(sizeof(double) * NBOU);
	double *out = (double *) malloc(sizeof(double) * NBOU);

	/*Initialize pointers*/
	init(reset, src, inputs, wIH, hNet, hOut, wHO, net);

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
	/*free(inputs);
	free(wIH);
	free(hNet);
	free(hOut);
	free(wHO);
	free(net);
	free(out);*/

	return CHARS[result];
}

/*============================================================================*/
