#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../BasicFunctions/basic.h"
#include "network.h"



/*========================= Declare global constants =========================*/

#define NBIN 785		//NBINPUTS			i : 784 + bias	| 28*28 px image
#define NBHN 70			//NBHIDDENNET		h1:
#define NBHO 71			//NBHIDDENOUT		h2: h1 + bias
#define NBOU 68			//NBOUTPUTS			o : letters, digits, a few other
#define ETA 0.5			//LEARNING RATE
#define NBTR 100		//NBTRAININGTURNS

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
double softmax(double net[], double out[], double maxnet)
{
	double sum = 0;
	for(int k = 0; k < NBOU; k++) {
		sum += exp(net[k] - maxnet + maxnet);
	}
	for(int j = 0; j < NBOU; j++) {
		out[j] = exp(net[j] - maxnet + maxnet) / sum;
	}
	return sum;
}

//softmax prime
void softmaxp(double net[], double netprime[], double sum, double maxnet)
{
	for(int x = 0; x < NBOU; x++) {
		//double ex = exp(net[x]) - maxnet + maxnet;
		//netprime[x] = (sum - ex) * ex / sum * sum;
		netprime[x] = (exp(net[x]-maxnet)/sum) * (1 - (exp(net[x]-maxnet)/sum));
	}
}

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
	}

	//max of net output array
	double maxnet = net[0];
	for (int i = 1; i < NBOU; i++) {
		if(net[i] > maxnet) {
			maxnet = net[i];
		}
	}

	// outputs activation
	double sum = softmax(net, out, maxnet);

	//DEBUG
	//printf("%g, %g, %g, %g, %g, %g, %g\n", inputs[0], wIH[0], hNet[0], hOut[1], wHO[0], net[0], out[0]);
	/*=========================================*/

	/*========== Backward Propagation =========*/
	double netprime[NBOU];
	softmaxp(net, netprime, sum, maxnet);

	if(mode == 't') {
		for(int o = 0; o < NBOU; o++) {
			for(int h = 0; h < NBHN; h++) {
				for(int i = 0; i < NBIN; i++) {
					//wIH
					double deltaIH = (out[o] - (((char)o==expected)?1:0))
									* netprime[o]
									* wHO[(h + 1) * NBOU + o]
									* sigp(hNet[h])
									* inputs[i];
					wIH[i * NBHN + h] -= deltaIH * ETA;
				}
				//wHO
				double deltaHO = (out[o] - (((char)o==expected)?1:0))
								* netprime[o]
								* hOut[h + 1];
				wHO[(h + 1) * NBOU + o] -= deltaHO * ETA;
			}
		}
		//bias wHO
		for(int o = 0; o < NBOU; o++) {
			double deltaHO = (out[o] - (((char)o==expected)?1:0))
							* netprime[o]
							* hOut[0];
			wHO[o] -= deltaHO * ETA;
		}
	}
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
	        char rnd = (char)(rand()%67);
	        while (rnd == expected) {
	            rnd = (char)(rand()%67);
	        }
	        expected = rnd;

	        //Create the corresponding path
	        char *PATH;
	        asprintf(&PATH, "./dataset_print/arial/%i.png", expected);

	        //Load the image
	        SDL_Surface *img;
	        img = IMG_Load(PATH);

			/*Initialize pointers*/
				//reset weights on first turn
			init((n == 1 ? 'y' : 'n'), img, inputs, wIH, hNet, hOut, wHO, net, 't');

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
	        //printf("Training %4i: expected %2i, got %2i\n", n, expected, result);
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
        if(result >= 0 && result <= 9) {i = result + 48;} //digits
        else if(result >= 10 && result <= 35) {i = result + 55;} //uppercase
        else if(result >= 36 && result <= 61) {i = result + 61;} //lowercase
        else if(result == 62) {i = 33;} // !
        else if(result == 63) {i = 39;} // '
        else if(result == 64) {i = 44;} // ,
        else if(result == 65) {i = 46;} // .
        else if(result == 66) {i = 58;} // :
        else if(result == 67) {i = 63;} // ?

		return i;
	}
}

/*============================================================================*/
