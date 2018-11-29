#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 785		//NBINPUTS			i : 784 + bias	| 28*28 px image
#define NBHN 100		//NBHIDDENNET		h1: 30
#define NBHO 101		//NBHIDDENOUT		h2: 30 + bias
#define NBOU 94			//NBOUTPUTS			o : 94 (char 33 to 126)
#define ETA 0.8			//LEARNING RATE

/*============================================================================*/



/*============================= Declare functions ============================*/

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
	char *PATH1 = "weights_IH";
	FILE *f1 = fopen(PATH1, "wb");
	fwrite(wIH, sizeof(double), sizeof(wIH), f1);
	fclose(f1);

	char *PATH2 = "weights_HO";
	FILE *f2 = fopen(PATH2, "wb");
	fwrite(wHO, sizeof(double), sizeof(wHO), f2);
	fclose(f2);
}

//load weigths
void load(double *wIH, double *wHO)
{
	char *PATH1 = "weights_IH";
	FILE *f1 = fopen(PATH1, "wb");
	fread(wIH, sizeof(double), sizeof(wIH), f1);
	fclose(f1);

	char *PATH2 = "weights_HO";
	FILE *f2 = fopen(PATH2, "wb");
	fread(wHO, sizeof(double), sizeof(wHO), f2);
	fclose(f2);
}

//initialize pointers
void init(char reset, SDL_Surface *src, double *inputs, double *wIH, \
	double *hNet, double *hOut, double *wHO, double *net)
{
	/*Weights*/
	if(reset == 'y') {
		for(int i = 0; i < NBIN; i++) {
			for(int h1 = 0; h1 < NBHN; h1++) {
				wIH[i * NBHN + h1] = (double)rand()/RAND_MAX*2.0-1.0;
			}
		}
		for(int h2 = 0; h2 < NBHO; h2++) {
			for(int o = 0; o < NBOU; o++) {
				wHO[h2 * NBOU + o] = (double)rand()/RAND_MAX*2.0-1.0;
			}
		}
	} else {
		load(wIH, wHO);
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
	for(int h1 = 0; h1 < NBHN; h1++) {
		for(int i = 0; i < NBIN; i++) {
			hNet[h1] += inputs[i] * wIH[i * NBHN + h1];
		}
		//printf("hNet %i : %g\n", h1, hNet[h1]); //DEBUG //HNET[0] = 0 WHY ???
	}

	// hidden layer activation
	for(int h1 = 0; h1 < NBHN; h1++) {
		hOut[h1+1] = sig(hNet[h1]);
	}

	// hidden layer -> outputs
	for(int o = 0; o < NBOU; o++) {
		for(int h2 = 0; h2 < NBHO; h2++) {
			net[o] += hOut[h2] * wHO[h2 * NBOU + o];
		}
		//printf("net %i : %g\n", o, net[o]); //DEBUG //NET[0] = -NAN WHY ???
	}

	// outputs activation
	softmax(net, out);
	/*=========================================*/

	/*========== Backward Propagation =========*/
	if(mode == 't') {
		// Output -> Hidden Layer
		// -= dErr/dOut * dOut/dNet * dNet/dWHO * ETA
		for(int h2 = 0; h2 < NBHO; h2++) {
			for(int o = 0; o < NBOU; o++) {
				wHO[h2 * NBOU + o] -= ((out[o] - (((char)(o+33)==expected)?1:0))
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
					deltaHO += ((out[o] - (((char)(o+33)==expected)?1:0))
								* softmaxp(out, h1 + 1, o)
					 			* wHO[(h1 + 1) * NBOU + o]);
				}
				wIH[i * NBHN + h1] -= (deltaHO * sigp(hNet[h1]) * inputs[i] * ETA);
			}
		}
	}
	/*=========================================*/
}

//main function to call | mode = (t)rain/eval | reset weigths = (y)es/no
char network(SDL_Surface *src, char mode, char reset, char expected)
{
	/*Initialize RNG*/
	time_t t;
	srand((unsigned) time(&t));

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
		//printf("%c : %g\n", i + 33, out[i]); //DEBUG
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

	return (char)(result+33);
}

/*============================================================================*/
