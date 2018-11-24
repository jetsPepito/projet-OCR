#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../ImageTreatment/image_treatment.h"
#include "../ImageTreatment/sdl.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 785		//NBINPUTS			: 784 + bias	| 28*28 px image
#define NBHN 784		//NBHIDDENNET		: 784
#define NBHO 785		//NBHIDDENOUT		: 784 + bias
#define NBOU 63			//NBOUTPUTS			: 62  + bias	| letters + digits
#define NBTR 100000		//NBTRAININGTURNS
#define ETA 0.1			//LEARNING RATE
#define PATH "nn.txt"	//SAVE FILE FOR THE WEIGTHS
#define CHARS[] {abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789}

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
	double *hNet, double *hOut, double *wHO, double *NET, double *OUT);
//main function to call | mode = (t)rain, (e)val | reset weigths = (y)es, (n)o
void network(SDL_surface *src, char mode, char reset, char expected);
//forward and backward propagation
void identify(char mode, double *inputs, double *wIH, double *hNet, \
	double *hOut, double *wHO, double *NET, double *OUT, char expected);

/*============================================================================*/



/*=================================== Code ===================================*/

double sig(double x)
{
	return (1 / (1 + exp(-x)));
}

double sigp(double x)
{
	double sigx = sig(x);
	return (sigx * (1 - sigx));
}

double softmax()
{
	// TODO
	return 0.0
}

double softmaxp()
{
	// TODO
	return 0.0
}

void save()
{
	// TODO
	return
}

void load()
{
	// TODO
	return
}

void network()
{
	// TODO
	return
}

void init()
{
	// TODO
	return
}

void identify()
{
	// TODO
	return
}

/*============================================================================*/
