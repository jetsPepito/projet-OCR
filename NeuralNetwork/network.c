#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 101		//NBINPUTS			i : 100 + bias	| 10*10 px image
#define NBHN 64			//NBHIDDENNET		h1:
#define NBHO 65			//NBHIDDENOUT		h2: h1 + bias
#define NBOU 52			//NBOUTPUTS			o : letters
#define ETA 0.25		//LEARNING RATE
#define NBTR 1000		//NBTRAININGTURNS

/*============================================================================*/



/*============================= Declare functions ============================*/

//sigmoid
double sig(double x)
{
	return (1 / (1 + exp(-x)));
}
/*
//sigmoid prime
double sigp(double sigx) //derivative of sig(x) in function of sig(x)
{
	return (sigx * (1 - sigx));
}
*/
//softmax
double softmax(double net[], double out[], double maxnet)
{
	double sum = 0;
	for(int k = 0; k < NBOU; k++) {
		sum += exp(net[k] - maxnet);
	}
	for(int j = 0; j < NBOU; j++) {
		out[j] = exp(net[j] - maxnet) / sum;
	}
	return sum;
}
/*
//softmax prime
void softmaxp(double net[], double sum, double maxnet, double netprime[])
{
	for(int i = 0; i < NBOU; i++) {
		netprime[i] = (exp(net[i]-maxnet)*(sum-exp(net[i]-maxnet)))/(sum*sum);
	}
}
*/
//save weigths
void save(double wIH[], double wHO[])
{
	char *PATH1 = "weights_IH";
	FILE *f1 = fopen(PATH1, "wb");
	fwrite(wIH, sizeof(double), NBIN * NBHN, f1);
	fclose(f1);

	char *PATH2 = "weights_HO";
	FILE *f2 = fopen(PATH2, "wb");
	fwrite(wHO, sizeof(double), NBHO * NBOU, f2);
	fclose(f2);
}

//load weigths
void load(double wIH[], double wHO[])
{
	char *PATH1 = "weights_IH";
	FILE *f1 = fopen(PATH1, "wb");
	fread(wIH, sizeof(double), NBIN * NBHN, f1);
	fclose(f1);

	char *PATH2 = "weights_HO";
	FILE *f2 = fopen(PATH2, "wb");
	fread(wHO, sizeof(double), NBHO * NBOU, f2);
	fclose(f2);
}

//initialize pointers
void init(char reset, SDL_Surface *src, double inputs[], double wIH[],
	double hNet[], double hOut[], double wHO[], double net[], char mode)
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
		if(mode != 't') {
			load(wIH, wHO);
		}
	}

	/*Inputs*/
	inputs[0] = 1.0; // bias
	Uint32 px;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	for(int x = 0; x < 10; x++) {
		for(int y = 0; y < 10; y++) {
			px = getpixel(src, x, y);
			SDL_GetRGB(px, src->format, &r, &g, &b);
			inputs[x * 10 + y + 1] = (r == 255) ? 0.0 : 1.0; // b = 1, w = 0
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
void identify(char mode, double inputs[], double wIH[], double hNet[],
	double hOut[], double wHO[], double net[], double out[], char expected)
{
	/*========== Forward Propagation ==========*/
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
		// outputs activation
		//out[o] = sig(net[o]);
	}
	//printf("%g\n", net[0]); //DEBUG

	//max of net output array
	double maxnet = net[0];
	for (int i = 1; i < NBOU; i++) {
		if(net[i] > maxnet) {
			maxnet = net[i];
		}
	}

	// outputs activation
	double sum = softmax(net, out, maxnet);
	sum = sum;

	//DEBUG
	printf("%g, %g, %g, %g, %g, %g, %g\n", inputs[1], wIH[1], hNet[1], hOut[1], wHO[1], net[1], out[1]);
	/*=========================================*/

	/*========== Backward Propagation =========*/
	//mode internet
	if(mode == 't') {
		//hidden layer -> input
		for(int i = 0; i < NBIN; i++) {
			for(int h1 = 0; h1 < NBHN; h1++) {
				double dEdHO = 0.0;
				for(int o = 0; o < NBOU; o++) {
					//hidden layer -> input
					dEdHO += ( (-((o == expected ? 1 : 0) - out[o]))
					 			* out[o] * (1 - out[o])
								* wHO[(h1 + 1) * NBOU + o] );
					//output -> hidden layer
					wHO[(h1 + 1) * NBOU + o] -= ( (-((o==expected?1:0)-out[o]))
											* out[o] * (1 - out[o])
											* hOut[h1 + 1]
											* ETA );
				}
				//hidden layer -> input
				wIH[i * NBHN + h1] -= ( dEdHO
									* hOut[h1 + 1] * (1 - hOut[h1 + 1])
									* inputs[i]
									* ETA );
			}
		}
		//output -> hidden layer (bias)
		for(int o = 0; o < NBOU; o++) {
			wHO[o] -= ( (-((o == expected ? 1 : 0) - out[o]))
						* out[o] * (1 - out[o])
						* ETA );
		}
	}
	/*//mode sander
	if(mode == 't') {
		double outprime = (exp(net[expected] - maxnet)
						* (sum - exp(net[expected] - maxnet))) / (sum*sum);

		//correct wHO bias
		double deltaHObias = ((-1)/out[expected]) * outprime;
		wHO[expected] -= deltaHObias * ETA;
		//correct wHO weights
		for(int h2 = 1; h2 < (NBHO*NBOU); h2++) {
			wHO[h2 * NBOU + expected] = deltaHObias * hOut[h2] * ETA;
		}

		//missing: wIH correction
	}
	*/
	/*=========================================*/
}

//main function to call | mode = (t)rain/eval
char network(SDL_Surface *src, char mode)
{
	/*Initialize RNG*/
	srand(time(NULL));

	/*Declare arrays*/
	double inputs[NBIN];
	double wIH[NBIN * NBHN];
	double hNet[NBHN];
	double hOut[NBHO];
	double wHO[NBHO * NBOU];
	double net[NBOU];
	double out[NBOU];

	//Training
	if(mode == 't'){
	    char expected = 0;
	    for (int n = 1; n <= NBTR; n++) {
	        //Select a random character
	        char rnd = (char)(rand()%51);
	        while (rnd == expected) {
	            rnd = (char)(rand()%51);
	        }
	        expected = rnd;

	        //Create the corresponding path
	        char *PATH;
	        asprintf(&PATH, "./dataset_print/arial_2/%i.bmp", expected);

	        //Load the image
	        SDL_Surface *img;
	        img = IMG_Load(PATH);

			/*Initialize pointers*/
				//reset weights on first turn
			init((n==1?'y':'n'), img, inputs, wIH, hNet, hOut, wHO, net, 't');

	       	/*Propagate*/
	   		identify('t', inputs, wIH, hNet, hOut, wHO, net, out, expected);

	   		/*Identify the character*/
	   		double mostprob = 0.0;
	   		int result = 0;
	   		for(int i = 0; i < NBOU; i++) {
	   			if(out[i] > mostprob) {
	   				mostprob = out[i];
	   				result = i;
	   			}
	   		}
			result += 0; //so it compiles
	        //Print the results
	        //printf("Training %4i: expected %2i, got %2i\n",n,expected,result);
	        free(img);
	        free(PATH);
	    }
		save(wIH, wHO);
		return 0;
	}
	else {
		/*Initialize pointers*/
			//do not reset weights
		init('n', src, inputs, wIH, hNet, hOut, wHO, net, mode);

		/*Propagate*/
		identify(mode, inputs, wIH, hNet, hOut, wHO, net, out, 0);

		/*Identify the character*/
		double mostprob = 0.0;
		int result = 0;
		for(int i = 0; i < NBOU; i++) {
			if(out[i] > mostprob) {
				mostprob = out[i];
				result = i;
			}
		}

		//adapt the character
        char i;
        if(result >= 0 && result <= 25) {i = result + 65;} //uppercase
        else {i = result + 71;} //lowercase

		return i;
	}
}

/*============================================================================*/
