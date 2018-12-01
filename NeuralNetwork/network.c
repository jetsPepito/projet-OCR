#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define SIZE 10				//IMG SIZE			size : 10 (x 10) px
#define NBIN 101			//NBINPUTS			i : size x size + bias
#define NBHN 35				//NBHIDDENNET		h1:
#define NBHO 36				//NBHIDDENOUT		h2: h1 + bias
#define NBOU 52				//NBOUTPUTS			o : letters
#define ETA 0.025			//LEARNING RATE

/*============================================================================*/



/*============================= Declare functions ============================*/

//sigmoid(x) -> return
double sig(double x)
{
	return (1 / (1 + exp(-x)));
}

//softmax(net) -> out ; return sum
double softmax(double net[], double out[], double maxnet)
{
	double sum = 0.0;
	for(int k = 0; k < NBOU; k++) {
		sum += exp(net[k] - maxnet);
	}
	for(int j = 0; j < NBOU; j++) {
		out[j] = exp(net[j] - maxnet) / sum;
	}
	return sum;
}

//save weigths
void save(double wIH[], double wHO[])
{
	int sizeIH = (NBIN * NBHN);
	int sizeHO = (NBHO * NBOU);

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
	int sizeIH = (NBIN * NBHN);
	int sizeHO = (NBHO * NBOU);

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

//initialize arrays
void init_train(SDL_Surface *src, double inputs[], double wIH[], double wHO[],
	double hNet[], double hOut[], double net[], int turn)
{
	//Inputs
	Uint32 px;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	for(int x = 0; x < SIZE; x++) {
		for(int y = 0; y < SIZE; y++) {
			px = getpixel(src, x, y);
			SDL_GetRGB(px, src->format, &r, &g, &b);
			inputs[x * SIZE + (y + 1)] = ((r == 255) ? 0.0 : 1.0); // b=1, w=0
		}
	}
	inputs[0] = 1.0; // bias

	//Weights
	if(turn == 1) {
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
	}

	//Hidden Layer
	for(int h1 = 0; h1 < NBHN; h1++) {
		hNet[h1] = 0.0;
	}
	hOut[0] = 1.0; // bias

	//Outputs
	for(int o = 0; o < NBOU; o++) {
		net[o] = 0.0;
	}
}

//initialize arrays
void init_eval(SDL_Surface *src, double inputs[], double wIH[], double wHO[],
	double hNet[], double hOut[], double net[])
{
	//Inputs
	Uint32 px;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	for(int x = 0; x < SIZE; x++) {
		for(int y = 0; y < SIZE; y++) {
			px = getpixel(src, x, y);
			SDL_GetRGB(px, src->format, &r, &g, &b);
			inputs[x * SIZE + (y + 1)] = ((r == 255) ? 0.0 : 1.0); // b=1, w=0
		}
	}
	inputs[0] = 1.0; // bias

	//Weights
	load(wIH, wHO);

	//Hidden Layer
	for(int h1 = 0; h1 < NBHN; h1++) {
		hNet[h1] = 0.0;
	}
	hOut[0] = 1.0; // bias

	//Outputs
	for(int o = 0; o < NBOU; o++) {
		net[o] = 0.0;
	}
}

//forward propagation
void identify(double inputs[], double wIH[], double hNet[],	double hOut[],
	double wHO[], double net[], double out[])
{
	// inputs -> hidden layer
	for(int h1 = 0; h1 < NBHN; h1++) {
		for(int i = 0; i < NBIN; i++) {
			hNet[h1] += inputs[i] * wIH[i * NBHN + h1];
		}
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
	}

	//max of net output array
	double maxnet = net[0];
	for (int i = 1; i < NBOU; i++) {
		if(net[i] > maxnet) {
			maxnet = net[i];
		}
	}

	// outputs activation
	softmax(net, out, maxnet);
}

//backward propagation
void correct(double inputs[], double wIH[], double hOut[],
	double wHO[], double out[], int expected)
{
	// CROSS-ENTROPY :

	//wHO
	for(int h2 = 0; h2 < NBHO; h2++) {
		wHO[h2 * NBOU + expected] -= ( ((-1)/out[expected])
									* out[expected] * (1 - out[expected])
									* hOut[h2]
									* ETA );
	}
	//wIH
	for(int i = 0; i < NBIN; i++) {
		for(int h1 = 0; h1 < NBHN; h1++) {
			wIH[i * NBHN + h1] -= ( ((-1)/out[expected])
								* out[expected] * (1 - out[expected])
								* wHO[(h1 + 1) * NBOU + expected]
								* hOut[h1 + 1] * (1 - hOut[h1 + 1])
								* inputs[i]
								* ETA );
		}
	}
}

//print the array's content
void __debug__(char mode, double array[])
{
	if(mode == 'i') { //inputs
		for (int i = 0; i < NBIN; i++) {
			printf("Input %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'w') { //wIH
		for (int i = 0; i < (NBIN * NBHN); i++) {
			printf("WIH %4i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'h') { //hNet
		for (int i = 0; i < NBHN; i++) {
			printf("HNet %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'H') { //hOut
		for (int i = 0; i < NBHO; i++) {
			printf("HOut %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'W') { //wHO
		for (int i = 0; i < (NBHO * NBOU); i++) {
			printf("WHO %4i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'n') { //net
		for (int i = 0; i < NBOU; i++) {
			printf("Net %3i : %g\n", i, array[i]);
		}
	}
	else if(mode == 'o') { //out
		for (int i = 0; i < NBOU; i++) {
			printf("Out %3i : %g\n", i, array[i]);
		}
	}
}

//train the network
void train(int nbtr)
{
	/*Initialize RNG*/
	srand(time(NULL));

	/*Prepare variables*/
	int expected = -1;
	char *PATH;

	/*Declare arrays*/
	double inputs[NBIN];
	double wIH[NBIN * NBHN];
	double hNet[NBHN];
	double hOut[NBHO];
	double wHO[NBHO * NBOU];
	double net[NBOU];
	double out[NBOU];

	/*Train*/
	for (int n = 1; n <= nbtr; n++)
	{
		//Select a random character
		int rnd = (char)(rand()%51);
		while (rnd == expected) {
			rnd = (char)(rand()%51);
		}
		expected = rnd;

		//Convert to char
		char expectedc = 255;
		if(expected >= 0 && expected <= 25) {expectedc = expected + 65;}
		else {expectedc = expected + 71;}
		expectedc += 0; //For cases where nothing is printed

		//Create the corresponding path
		asprintf(&PATH, "./dataset_print/arial_2/%i.bmp", expected);

		//Load the image
		SDL_Surface *img;
		img = IMG_Load(PATH);

		//Initialize arrays
		init_train(img, inputs, wIH, wHO, hNet, hOut, net, n);

		//Propagate
		identify(inputs, wIH, hNet, hOut, wHO, net, out);

		//Back-Propagate
		correct(inputs, wIH, hOut, wHO, out, expected);

		//Identify the character
		double mostprob = 0.0;
		int result = 0;
		for(int i = 0; i < NBOU; i++) {
			if(out[i] > mostprob) {
				mostprob = out[i];
				result = i;
			}
		}

		//Convert to char
		char resultc = 255;
		if(result >= 0 && result <= 25) {resultc = result + 65;}
		else {resultc = result + 71;}
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
char network(SDL_Surface *src)
{
	/*Initialize RNG*/
	srand(time(NULL));

	/*Declare arrays*/
	double inputs[NBIN];
	double wIH[NBIN * NBHN];
	double wHO[NBHO * NBOU];
	double hNet[NBHN];
	double hOut[NBHO];
	double net[NBOU];
	double out[NBOU];

	/*Initialize arrays*/
	init_eval(src, inputs, wIH, wHO, hNet, hOut, net);

	/*Propagate*/
	identify(inputs, wIH, hNet, hOut, wHO, net, out);

	/*Identify the character*/
	double mostprob = 0.0;
	int result = 0;
	for(int i = 0; i < NBOU; i++) {
		if(out[i] > mostprob) {
			mostprob = out[i];
			result = i;
		}
	}

	/*Convert to char*/
    char resultc;
    if(result >= 0 && result <= 25) {resultc = result + 65;}
    else {resultc = result + 71;}

	return resultc;
}

/*============================================================================*/
