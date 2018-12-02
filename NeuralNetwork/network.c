#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include "network.h"
#include "../BasicFunctions/basic.h"

#define FILENAME "network_save"
#define MAX_SIZE 100000


/* INIT FUNCTIONS */


void free_layer(Layer** l, size_t NBL)
{
	for (size_t i = 0 ; i < NBL; i++) {
		free(l[i]->weights);
		free(l[i]->lastWeights);

       	free(*(l + i));

		//free(l[i]);
	}

	free(l);
}


void free_network(Network* network)
{
	free_layer(network->inputs, network->NBI);
	free_layer(network->hiddens, network->NBH);
	free_layer(network->outputs, network->NBO);

	//free(network);
}


void init_inputs(float *inputs, SDL_Surface *surface)
{
	int width = surface->w;
	int height = surface->h;
	Uint32 pixel;
	Uint8 r;
	Uint8 g;
	Uint8 b;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			pixel = getpixel(surface, j, i);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			inputs[j*height+i] =(float) r==0;
		}
	}
}


void init_input_layer(Network* network, float* value)
{
	for (size_t i = 0; i < network->NBI; i++) {
		network->inputs[i]->net = value[i];
		network->inputs[i]->out = value[i];
	}
}


void init_layer(Layer** layers, size_t NBLayer, size_t NBLink)
{
	for(size_t i = 0 ; i < NBLayer; i++) {
		layers[i] = (Layer*)calloc(1,sizeof(Layer));
		layers[i]->bias = 0;
		layers[i]->lastBias = 0;

		layers[i]->net = 0;
		layers[i]->out = 0;
		layers[i]->weights = (float*)calloc(NBLink, sizeof(float));
		layers[i]->lastWeights = (float*)calloc(NBLink, sizeof(float));
	}
}


void link_layers(Layer** l1, size_t NBL1, size_t NBL2)
{
	srand((unsigned int) time(NULL));
	float init = 1/sqrt(900);

	for(size_t i = 0 ; i < NBL1; i++) {
		for(size_t j = 0 ; j < NBL2; j++) {
			l1[i]->weights[j] = (float)(rand()*2.0f*init)
								/(float)(RAND_MAX)
								-init;
			l1[i]->lastWeights[j]= l1[i]->weights[j];
		}
	}
}


void init_links(Network* n)
{
	if (&(n->hiddens) == NULL) {
		printf("No hiden");
		link_layers(n->inputs, n->NBI, n->NBO);
	} else {
		link_layers(n->inputs, n->NBI, n->NBH);
		link_layers(n->hiddens , n->NBH, n->NBO);
	}
}


void init_network(Network* network, size_t NBI, size_t NBH, size_t NBO)
{
	network->NBI = NBI;
	network->NBH = NBH;
	network->NBO = NBO;

	network->inputs = (Layer**)calloc(NBI, sizeof(Layer*));
	network->hiddens = (Layer**)calloc(NBH, sizeof(Layer*));
   	network->outputs = (Layer**)calloc(NBO, sizeof(Layer*));

	init_layer(network->inputs, NBI, NBH);
	init_layer(network->hiddens, NBH, NBO);
	init_layer(network->outputs, NBO, 0);

	init_links(network);
}


/* TOOLBOX FUNCTIONS */


float sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}


float sigmoid_prime(float x)
{
	return (sigmoid(x) * (1-sigmoid(x)));
}


float calculateError(Network* n, float* expected)
{
	float error = 0;
	for (size_t i = 0; i <  n->NBO; i++) {
		error += fabs(n->outputs[i]->out - expected[i]);
	}

	return error;// / n->NBO;
}


void calculateGradient(Layer** l1, size_t NBL1, Layer** l2, size_t NBL2)
{
	for(size_t i = 0; i < NBL1 ;i++)	{
		for(size_t j = 0; j < NBL2 ; j++) {
			l1[i]->gradient += l1[i]->weights[j]*l2[j]->gradient ;
		}
		l1[i]->gradient = l1[i]->gradient*sigmoid_prime(l1[i]->net);
	}
}


void resetGrad(Layer** l, size_t NBL)
{
	for (size_t i = 0; i < NBL; i++) {
		l[i]->gradient = 0;
	}
}


/* SAVE AND LOAD */


void save_layers(Layer** l, size_t NBL, size_t NBW, FILE* file)
{
	// bias
	fputs("(", file);
	for(size_t i = 0; i < NBL; i++) {
		if (i == NBL - 1) {
			fprintf(file, "%f", l[i]->bias);
		} else {
			fprintf(file, "%f,", l[i]->bias);
		}
	}
	fputs(")\n", file);

	//weights
	if (NBW != 0) {
		for (size_t i = 0; i < NBL ; i++) {
			fputc('(', file);
			for(size_t j = 0; j < NBW; j++) {
				if (j == NBW - 1) {
					fprintf(file, "%f", l[i]->weights[j]);
				} else {
					fprintf(file, "%f,", l[i]->weights[j]);
				}
			}
			fputs(")\n", file);
		}
	}
}


int save_network(Network* network)
{
	FILE* file = NULL;
	file = fopen(FILENAME,"w+");

	save_layers(network->inputs, network->NBI,network->NBH, file);
	save_layers(network->hiddens, network->NBH, network->NBO, file);
	save_layers(network->outputs, network->NBO, 0, file);

	fclose(file);

	return 0;
}


void load_layers(Layer** l, size_t NBL, size_t NBW, FILE* file)
{
	char chain[MAX_SIZE];
	float actualNumber = 0, dec = 0.1;
	size_t i = 0, j = 0;
	char sign = 0, decimal = 0;

	if (fgets(chain, MAX_SIZE, file) == NULL) {
        printf("NOT ENOUGH LINE\n");
	}

	for (i = 0; i < NBL; i++)
	{
		j++;
		if (chain[j] == '-') {
			sign = 1;
			j++;
		}

		decimal = 0;
	    actualNumber = 0;
        dec = 0.1;

		for(; chain[j] != ',' && chain[j] != ')' && chain[j] != '\n'; j++) {
			if (chain[j] == '.') {
				decimal = 1;
				j++;
			}
			if(decimal) {
				actualNumber += (((float) chain[j] - 48)) * dec;
				dec *= 0.1;
			}
			else {
				actualNumber = actualNumber * 10 + (((float) chain[j] - 48));
			}
		}

		if (sign == 1) {
			actualNumber *= -1;
		}

		l[i]->bias = actualNumber;
        sign = 0;
	}


	for (i = 0; i < NBL; i++)
	{
		if (fgets(chain, MAX_SIZE, file) == NULL)
            printf("NOT ENOUGH LINE\n");

        j = 0;
		for (size_t k = 0; k < NBW; k++)
		{
            j++;

            if (chain[j] == '-'){
                sign = 1;
                j++;
            }

            decimal = 0;
            actualNumber = 0;
            dec = 0.1;

            for(; chain[j] != ',' && chain[j] != ')' && chain[j] != '\n'; j++) {
                if (chain[j] == '.') {
                    decimal = 1;
                    j++;
                }
                if(decimal) {
                    actualNumber += (((float) chain[j] - 48)) * dec;
                    dec *= 0.1;
                }
                else {
                    actualNumber = actualNumber * 10 + (((float) chain[j] - 48));
				}
            }

            if (sign == 1) {
                actualNumber *= -1;
			}

            l[i]->weights[k] = actualNumber;
            sign = 0;
		}
	}
}


int load_network(Network* network)
{
	FILE* file = NULL;
	file = fopen(FILENAME, "r");

	if(file == NULL){
		printf("Loading failed...\n");
		return 1;
	}

	load_layers(network->inputs, network->NBI,network->NBH, file);
	load_layers(network->hiddens, network->NBH, network->NBO, file);
	load_layers(network->outputs, network->NBO, 0, file);
	fclose(file);

	return 0;
}


/* FORWARD PASS FUNCTIONS */


void sumWeight(Layer** l, size_t NBL, Layer** l2, size_t NBL2)
{
	for(size_t i = 0 ; i < NBL; i++) {
		for(size_t j = 0 ; j < NBL2; j++) {
			l2[j]->net += l[i]->out
							* l[i]->weights[j];
		}
	}
}


void activate(Layer** l, size_t NBL)
{
	for(size_t i = 0 ; i < NBL; i++) {
		l[i]->net += l[i]->bias;
		l[i]->out = sigmoid(l[i]->net);
	}
}


void forward_pass(Network* n)
{
	for(size_t i = 0 ; i < n->NBH; i++) {
		n->hiddens[i]->net = 0;
		n->hiddens[i]->out = 0;
	}
	for(size_t i = 0 ; i < n->NBO; i++){
		n->outputs[i]->net = 0;
		n->outputs[i]->out = 0;
	}

	sumWeight(n->inputs, n->NBI, n->hiddens, n->NBH);

	activate(n->hiddens, n->NBH);

	sumWeight(n->hiddens, n->NBH, n->outputs, n->NBO);

	activate(n->outputs, n->NBO);
}


/* BACK PROPAGATION FUNCTIONS */


void update(Layer** l1, size_t NBL1, Layer** l2, size_t NBL2, float ETA)
{
	for(size_t i = 0; i < NBL1 ;i++) {
		for(size_t j = 0; j < NBL2 ; j++) {
			l1[i]->lastWeights[j] = l1[i]->lastWeights[j] * 0.01f
					- ETA * l2[j]->gradient * l1[i]->out;

			l1[i]->weights[j] += l1[i]->lastWeights[j];
		}
	}
}


void update_bias(Layer** layers, size_t NBL, float ETA)
{
	for (size_t i = 0; i < NBL; i++) {
		layers[i]->lastBias = 0.01f * layers[i]->lastBias
							- layers[i]->gradient * ETA;
		layers[i]->bias += layers[i]->lastBias ;
	}
}


void backward_prop(Network* n, float ETA)
{
	//change the weights
	update(n->hiddens, n->NBH, n->outputs, n->NBO,ETA);
	update(n->inputs, n->NBI, n->hiddens, n->NBH,ETA);

	//change the bias
	update_bias(n->outputs, n->NBO, ETA);
	update_bias(n->hiddens, n->NBH, ETA);
}


/* LEARNING FUNCTIONS */


float train(Network *n,char *path, int expected, float ETA)
{
	float *exOut = calloc(n->NBO, sizeof(float));
	float error = 100;

	resetGrad(n->outputs, n->NBO);
	resetGrad(n->hiddens, n->NBH);
	resetGrad(n->inputs, n->NBI);

	//init inputs
	exOut[expected] = 1;
	float *ex = malloc(sizeof(float)*n->NBI);
	SDL_Surface *s = SDL_LoadBMP(path);
	init_inputs(ex,s);

	//forward pass
	init_input_layer(n, ex);
	forward_pass(n);

	for (size_t i = 0; i < n -> NBO ; i++) {
		Layer* l = n->outputs[i];
        l->gradient +=  (l->out - exOut[i]) *  sigmoid_prime(l->net);
	}

	//error calculation
	error = calculateError(n, exOut);

	exOut[expected] = 0;

	free(ex);

	//backward propagation
	calculateGradient(n->hiddens, n->NBH, n->outputs, n->NBO);
	backward_prop(n, ETA);


	free(exOut);
	return error;
}


/* EVAL FUNCTION */


char analyze(SDL_Surface *s, Network *n)
{
	float *inputs = malloc(sizeof(float)*n->NBI);
	init_inputs(inputs, s);

	init_input_layer(n, inputs);
	forward_pass(n);

	char *output = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int max = 0;
	for(size_t x = 0; x < n->NBO; x++) {
		if(n->outputs[x]->out > n->outputs[max]->out) {
			max = x;
		}
	}
	free(inputs);
	return *(output + max);
}


/* SUCCESS CALCULATIONS */


void calcSuccess(Network *n)
{
	double success_rate = 0.0;
    for (int j = 0; j < 52; j++)
    {
        //adapt the character
        char i;
        if(j >= 0 && j <= 25) {i = j + 65;} //uppercase
        else {i = j + 71;} //lowercase

        //Create the path
        char *PATH;
        asprintf(&PATH, "./dataset_print/arial_2/%i.bmp", j);

        //Load the image
        SDL_Surface *img;
        img = SDL_LoadBMP(PATH);

        //Call the neural network
        char id = analyze(img, n);

        //Check for success rate
        if(id == (char)i) {
            success_rate++;
        }

        SDL_FreeSurface(img);
        free(PATH);

		//Print the results
        printf("\texpected %c, got %c\n", i, id);
    }
	printf("Success rate : %g\n", (success_rate / 52));
}


/* TRAINING STARTER */


void createNetwork(Network *n, int NBO, int NBI, int NBH)
{
   	init_network(n, NBI, NBH, NBO);
	char *path;

	float error = 100;
	int i = 0;
	while(error > 0.1 || i < 1000) {
		i++;
		int expected = (char)(rand()%51);
		asprintf(&path, "/NeuralNetwork/dataset_print/arial_2/%i.bmp", expected);
		error = train(n, path, expected, 1.0f);
	}
	save_network(n);
}


/* PUBLIC FUNCTION */

char network(SDL_Surface *img, Network *n)
{
	return analyze(img, n);
}
