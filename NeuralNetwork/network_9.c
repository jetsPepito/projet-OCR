#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define SIZ 10				//IMG SIZE			size : 10 (x 10) px
#define NBI 100				//NBINPUTS			i : size x size + bias
#define NBH 60				//NBHIDDENNET		h1:
#define NBO 9				//NBOUTPUTS			o : letters
#define ETA 0.1 			//LEARNING RATE
#define ALP 0.001			//MOMENTUM

/*============================================================================*/



/*============================= Declare functions ============================*/

//print the array's content
void __debug__(char mode, double array[])
{
	if(mode == 'i') { //inputs
		for (int i = 0; i <= NBI; i++) {
			printf("Input %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'w') { //wIH
		for (int i = 0; i < (NBI * NBH) + NBH; i++) {
			printf("WIH %4i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'h') { //hNet
		for (int i = 0; i < NBH; i++) {
			printf("HNet %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'H') { //hOut
		for (int i = 0; i <= NBH; i++) {
			printf("HOut %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'W') { //wHO
		for (int i = 0; i < (NBH * NBO) + NBO; i++) {
			printf("WHO %4i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'n') { //net
		for (int i = 0; i < NBO; i++) {
			printf("Net %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'o') { //out
		for (int i = 0; i < NBO; i++) {
			printf("Out %3i : %g\n", i, array[i]);
		}
	}
}

//sigmoid(x) -> return
double sig(double x)
{
	return (1 / (1 + exp(-x)));
}

//sigprime(x) -> return
double sigprime(double x)
{
	return sig(x) * (1 - sig(x));
}

//softmax(net[i]) -> out[i] ; return sum
double softmax(double net[], double out[], double maxnet)
{
	double sum = 0.0;
	for(int k = 0; k < NBO; k++) {
		sum += exp(net[k] - maxnet);
	}
	for(int j = 0; j < NBO; j++) {
		out[j] = exp(net[j] - maxnet) / sum;
	}
	return sum;
}

//save weigths
void save(double wIH[], double wHO[])
{
	int sizeIH = ((NBI+1) * NBH);
	int sizeHO = ((NBH+1) * NBO);

	double weights[sizeIH + sizeHO];
	for(int i = 0; i < sizeIH; i++) {
		weights[i] = wIH[i];
	}
	for(int i = sizeIH; i < sizeHO; i++) {
		weights[i] = wHO[i - sizeIH];
	}

	FILE *f = fopen("weights_save", "wb");
	fwrite(weights, sizeof(double)*(sizeIH + sizeHO), 1, f);
	fclose(f);
}

//load weigths
void load(double wIH[], double wHO[])
{
	int sizeIH = ((NBI+1) * NBH);
	int sizeHO = ((NBH+1) * NBO);

	double weights[sizeIH + sizeHO];
	FILE *f = fopen("weights_save", "rb");
	fread(weights, sizeof(double)*(sizeIH + sizeHO), 1, f);
	fclose(f);

	for(int i = 0; i < sizeIH; i++) {
		wIH[i] = weights[i];
	}
	for(int i = sizeIH; i < sizeHO; i++) {
		wHO[i - sizeIH] = weights[i];
	}
}

//initialise inputs
void init_inputs(SDL_Surface *img, double inputs[])
{
	//Inputs
	Uint32 px;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	for(int x = 0; x < SIZ; x++) {
		for(int y = 0; y < SIZ; y++) {
			px = getpixel(img, x, y);
			SDL_GetRGB(px, img->format, &r, &g, &b);
			inputs[x * SIZ + y] = ((r == 255) ? 0.0 : 1.0); // b=1, w=0
		}
	}
	inputs[NBI] = 1.0; // bias
}

//initialize arrays
void init_train(double hNet[], double hOut[], double gradH[],
	double net[], double out[], double gradO[])
{
	//Hidden Layer
	for(int h = 0; h < NBH; h++) {
		hNet[h] = 0.0;
		hOut[h] = 0.0;
		gradH[h] = 0.0;
	}
	hOut[NBH] = 1.0; // bias

	//Outputs
	for(int o = 0; o < NBO; o++) {
		net[o] = 0.0;
		out[o] = 0.0;
		gradO[o] = 0.0;
	}
}

//initialize arrays
void init_eval(double wIH[], double wHO[],
	double hNet[], double hOut[], double net[], double out[])
{
	//Weights
	load(wIH, wHO);

	//Hidden Layer
	for(int h = 0; h < NBH; h++) {
		hNet[h] = 0.0;
		hOut[h] = 0.0;
	}
	hOut[NBH] = 1.0; // bias

	//Outputs
	for(int o = 0; o < NBO; o++) {
		net[o] = 0.0;
		out[o] = 0.0;
	}
}

//forward propagation
void identify(double inputs[], double wIH[], double hNet[],	double hOut[],
	double wHO[], double net[], double out[])
{
	// inputs -> hidden layer
	for(int h = 0; h < NBH; h++) {
		for(int i = 0; i <= NBI; i++) {
			hNet[h] += inputs[i] * wIH[i * NBH + h];
		}
	}

	// hidden layer activation
	for(int h = 0; h < NBH; h++) {
		hOut[h] = sig(hNet[h]);
	}

	// hidden layer -> outputs
	for(int o = 0; o < NBO; o++) {
		for(int h = 0; h <= NBH; h++) {
			net[o] += hOut[h] * wHO[h * NBO + o];
		}
	}

	// outputs activation
	for(int o = 0; o < NBO; o++) {
		out[o] = sig(net[o]);
	}
	/*
	//max of net output array
	double maxnet = net[0];
	for (int i = 1; i < NBO; i++) {
		if(net[i] > maxnet) {
			maxnet = net[i];
		}
	}
	softmax(net, out, maxnet);
	*/
}

void gradCalc(double hNet[], double wHO[], double gradH[],
	double net[], double out[], double gradO[], double target[])
{
	for(int o = 0; o < NBO; o++) {
		gradO[o] = (out[o] - target[o]) * sigprime(net[o]);
	}

	for(int h = 0; h <= NBH; h++) {
		for(int o = 0; o < NBO; o++) {
			gradH[h] += wHO[h * NBO + o] * gradO[o];
		}
		gradH[h] *= sigprime(hNet[h]);
	}
}

//backward propagation
void correct(double inputs[], double wIH[], double prevwIH[], double gradH[],
	double hOut[], double wHO[], double prevwHO[], double gradO[])
{
	// CROSS-ENTROPY :
	/*
	//wIH
	for(int i = 0; i < NBI; i++) {
		for(int h = 0; h <= NBH; h++) {
			wIH[i * NBH + h] -= ( ((-1)/out[expected])
								* out[expected] * (1 - out[expected])
								* wHO[h * NBO + expected]
								* (hNet[h] > 0 ? 1 : 0)
								* inputs[i]
								* ETA );
		}
	}
	//wHO
	for(int h = 0; h < NBH; h++) {
		wHO[h * NBO + expected] -= ( ((-1)/out[expected])
									* out[expected] * (1 - out[expected])
									* hOut[h]
									* ETA );
	}
	*/
	for(int h = 0; h <= NBH; h++) {
		for(int o = 0; o < NBO; o++) {
			prevwHO[h*NBO+o] = prevwHO[h*NBO+o]*ALP - hOut[h]*gradO[o]*ETA;
			wHO[h*NBO+o] += prevwHO[h*NBO+o];
		}
	}
	for(int i = 0; i <= NBI; i++) {
		for(int h = 0; h < NBH; h++) {
			prevwIH[i*NBH+h] = prevwIH[i*NBH+h]*ALP - inputs[i]*gradH[h]*ETA;
			wIH[i*NBH+h] += prevwIH[i*NBH+h];
		}
	}
}

//train the network
void train(int nbtr)
{
	/*Initialize RNG*/
	srand(time(NULL));

	/*Declare arrays*/
	double inputs[NBI+1];
	double wIH[(NBI+1) * NBH];
	double wHO[(NBH+1) * NBO];
	double hNet[NBH];
	double hOut[NBH+1];
	double net[NBO];
	double out[NBO];
	/*Learning arrays*/
	double prevwIH[(NBI+1) * NBH];
	double prevwHO[(NBH+1) * NBO];
	double gradH[NBH+1];
	double gradO[NBO];
	/*initialize weights within [-0.5,0.5]*/
	for(int i = 0; i <= NBI; i++) {
		for(int h = 0; h < NBH; h++) {
			wIH[i * NBH + h] = rand()/((double)RAND_MAX) - 0.5;
			prevwIH[i * NBH + h] = 0.0;
		}
	}
	for(int h = 0; h <= NBH; h++) {
		for(int o = 0; o < NBO; o++) {
			wHO[h * NBO + o] = rand()/((double)RAND_MAX) - 0.5;
			prevwHO[h * NBO + o] = 0.0;
		}
	}

	/*Train*/
	//Prepare variables
	int expected = -1;
	char *PATH;
	//Train
	for (int n = 1; n <= nbtr; n++)
	{
		//Select a random character
		int rnd = (char)(rand()%9);
		expected = rnd;

		//Build target array
		double target[NBO];
		for(int o = 0; o < NBO; o++) {
			target[o] = 0.0;
		}
		target[expected] = 1.0;

		//Convert to char
		char expectedc = 255;
		if(expected == 0) {expectedc = 'L';}
		else if(expected == 1) {expectedc = 'e';}
		else if(expected == 2) {expectedc = 'i';}
		else if(expected == 3) {expectedc = 'm';}
		else if(expected == 4) {expectedc = 'o';}
		else if(expected == 5) {expectedc = 'p';}
		else if(expected == 6) {expectedc = 'r';}
		else if(expected == 7) {expectedc = 's';}
		else {expectedc = 'u';}
		expectedc += 0; //For cases where nothing is printed

		//Create the corresponding path
		asprintf(&PATH, "./dataset_print/arial/%i.bmp", expected);

		//Load the image
		SDL_Surface *img;
		img = IMG_Load(PATH);

		//Initialize inputs
		init_inputs(img, inputs);

		//Initialize arrays
		init_train(hNet, hOut, gradH, net, out, gradO);

		//Propagate
		identify(inputs, wIH, hNet, hOut, wHO, net, out);

		//Calculate gradients
		gradCalc(hNet, wHO, gradH, net, out, gradO, target);

		//Back-Propagate
		correct(inputs, wIH, prevwIH, gradH, hOut, wHO, prevwHO, gradO);

		//Identify the character
		double mostprob = 0.0;
		int result = 0;
		for(int i = 0; i < NBO; i++) {
			if(out[i] > mostprob) {
				mostprob = out[i];
				result = i;
			}
		}

		//Convert to char
		char resultc = 255;
		if(result == 0) {resultc = 'L';}
		else if(result == 1) {resultc = 'e';}
		else if(result == 2) {resultc = 'i';}
		else if(result == 3) {resultc = 'm';}
		else if(result == 4) {resultc = 'o';}
		else if(result == 5) {resultc = 'p';}
		else if(result == 6) {resultc = 'r';}
		else if(result == 7) {resultc = 's';}
		else {resultc = 'u';}
		resultc += 0; //For cases where nothing is printed

		//Print the results
		//printf("Training %4i: expected %c, got %c\n",n,expectedc,resultc);
		SDL_FreeSurface(img);
		free(PATH);
	}

	/*Save weights*/
	save(wIH, wHO);
}

//main function to call
char network(SDL_Surface *img)
{
	/*Initialize RNG*/
	srand(time(NULL));

	/*Declare arrays*/
	double inputs[NBI+1];
	double wIH[(NBI+1) * NBH];
	double wHO[(NBH+1) * NBO];
	double hNet[NBH];
	double hOut[NBH+1];
	double net[NBO];
	double out[NBO];

	//Initialize inputs
	init_inputs(img, inputs);

	//Initialize arrays
	init_eval(wIH, wHO, hNet, hOut, net, out);

	//Propagate
	identify(inputs, wIH, hNet, hOut, wHO, net, out);

	/*Identify the character*/
	double mostprob = 0.0;
	int result = 0;
	for(int i = 0; i < NBO; i++) {
		if(out[i] > mostprob) {
			mostprob = out[i];
			result = i;
		}
	}

	/*Convert to char*/
    char resultc;
    if(result == 0) {resultc = 'L';}
	else if(result == 1) {resultc = 'e';}
	else if(result == 2) {resultc = 'i';}
	else if(result == 3) {resultc = 'm';}
	else if(result == 4) {resultc = 'o';}
	else if(result == 5) {resultc = 'p';}
	else if(result == 6) {resultc = 'r';}
	else if(result == 7) {resultc = 's';}
	else {resultc = 'u';}

	return resultc;
}

/*============================================================================*/
