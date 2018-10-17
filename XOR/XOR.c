#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*Declare functions*/
double sig(double x);
double sigp(double x);
double predict(int i1, int i2, int target, double** w1, double* w2);
void train(int n, double** w1, double* w2, int argc);
void test(int i1, int i2, double** w1, double* w2);

/*Declare global constants*/
int NBIN = 3;		//NBINPUTS 		(i): 2 + bias
int NBHN = 3;		//NBHIDDENNET	(h): 3
int NBHO = 4;		//NBHIDDENOUT	(h): 3 + bias
int NBTR = 150000;	//NBTRAININGTURNS
double ETA = 0.1;	//LEARNING RATE

int main(int argc, const char* argv[]) //args: i1, i2 ; non-binary = 0
{
	/*Manual*/
	if(argc > 1)
	{
		if((strcmp(argv[1],"help")==0) || (strcmp(argv[1],"h")==0))
		{
			printf("\n$./XOR <input1> <input2>\n\n");
			printf("<input1>: \"help\" or \"h\" for manual ; ");
			printf("\"1\" or \"0\" for first input of the test\n");
			printf("<input2>: \"1\" or \"0\" for second input of the test\n\n");
			printf("*Empty inputs will be randomly generated\n");
			printf("*Wrong inputs will be treated as \"0\"\n");
			printf("*Extra inputs will be ignored\n");
			printf("*Absence of input will print training and truth table\n\n");

			return EXIT_SUCCESS;
		}
	}

	/*Initialize RNG*/
	time_t t;
	srand((unsigned) time(&t));

	/*Declare weigths*/
	double **w1;	//w1[hnet][input]
	w1 = (double **) malloc(NBHN*sizeof(double *));
	for(int h = 0; h < NBHN; h++)
		w1[h] = (double *) malloc(NBIN*sizeof(double));

	double *w2;		//w2[hout]
	w2 = (double *) malloc(NBHO*sizeof(double));

	/*Initialize weigths with random values from -1 to 1*/
	for(int h = 0; h < NBHN; h++)
	{
		w1[h][0] = 0.0;
		for(int i = 1; i < NBIN; i++)
		{
			w1[h][i] = (double)rand()/RAND_MAX*2.0-1.0;
		}
	}
	w2[0] = 0.0;
	for(int h = 1; h < 3; h++)
	{
		w2[h] = (double)rand()/RAND_MAX*2.0-1.0;
	}

	/*Train and test the result*/
	printf("\nTraining...\n");
	train(NBTR, w1, w2, argc);

	if(argc > 1)
	{
		int i1 = (strcmp(argv[1],"1")==0) ? 1 : 0;
		int i2;
		if(argc > 2)
		{
			i2 = (strcmp(argv[2],"1")==0) ? 1 : 0;
		}
		else
		{
			i2 = rand() % (1 + 1 - 0) + 0;
		}
		printf("\n");
		test(i1, i2, w1, w2);
		printf("\n");
	}
	else
	{
		printf("\n");
		test(0, 0, w1, w2);
		test(0, 1, w1, w2);
		test(1, 0, w1, w2);
		test(1, 1, w1, w2);
		printf("\n");
	}

	/*"It just works" - Todd Howard*/
	return EXIT_SUCCESS;
}


/*Sigmoid function*/
double sig(double x)
{
	return (1 / (1 + exp(-x)));
}
double sigp(double x)
{
	double sigx = sig(x);
	return (sigx * (1 - sigx));
}


/*Neural network*/
//target = -1 -> no backprop
double predict(int i1, int i2, int target, double** w1, double* w2)
{
	/*Initialize inputs and variables*/
	double targ = (target == 1) ? 1.0 : 0.0;
	double inputs[3] = {1.0,((i1 == 1) ? 1.0 : 0.0),((i2 == 1) ? 1.0 : 0.0)};
	double hnet[3] = {0.0, 0.0, 0.0};
	double hout[4] = {1.0, 0.0, 0.0, 0.0};
	double net = 0.0;

	/*Forward propagation*/
	//Inputs -> Hidden layer
	for(int h = 0; h < NBHN; h++)
	{
		for(int i = 0; i < NBIN; i++)
		{
			hnet[h] += inputs[i] * w1[h][i];
		}
	}
	//Hidden layer activation
	for(int h = 0; h < NBHN; h++)
		hout[h+1] = sig(hnet[h]);

	//Hidden layer -> Output
	for(int h = 0; h < NBHO; h++)
		net += hout[h] * w2[h];
	//Output activation
	double out = sig(net);

	/*Backward propagation*/
	if(target != -1)
	{
		//Output -> Hidden Layer
		for(int h = 0; h < NBHO; h++)
		{
			//			   derr/dout    dout/dnet   dnet/dw2
			double delta = (out-targ) * sigp(net) * hout[h] * ETA;
			w2[h] -= delta;
		}

		//Hidden layer -> Inputs
		for(int h = 0; h < NBHN; h++)
		{
			for(int i = 0; i < NBIN; i++)
			{
				//			   derr/dout    dout/dnet  dnet/dhout  dhout/dhnet
				double delta = (out-targ) * sigp(net) * w2[h+1] * sigp(hnet[h])
								* inputs[i] * ETA;
				//				dhnet/dw1
				w1[h][i] -= delta;
			}
		}
	}
	return out;
}


/*Training loop (with backpropagation)*/
void train(int n, double** w1, double* w2, int argc)
{
	for(int i = 0; i < n; i++)
	{
		int i1 = rand() % (1 + 1 - 0) + 0;
		int i2 = rand() % (1 + 1 - 0) + 0;
		int targ = (i1 == i2) ? 0 : 1;

		double prediction = predict(i1, i2, targ, w1, w2);

		/*Purely for showcase, remove to optimize*/
		if(argc <= -5)//1)
		{
			if(i <= 10)
				printf("{%i,%i}: %g , expected %i\n", i1, i2, prediction, targ);
			if(i == 11)
				printf("    (...)\n");
			if(i >= n - 11)
				printf("{%i,%i}: %g , expected %i\n", i1, i2, prediction, targ);
		}
	}
}


/*Evaluation (without backpropagation*/
void test(int i1, int i2, double** w1, double* w2)
{
	double prediction = predict(i1, i2, -1, w1, w2);
	if (prediction >= 0.85)
		printf("%i XOR %i = 1 	(prediction: %g)\n", i1, i2, prediction);
	else if (prediction <= 0.15)
		printf("%i XOR %i = 0 	(prediction: %g)\n", i1, i2, prediction);
	else
		printf("%i XOR %i = ? 	(INNACURATE: %g)\n", i1, i2, prediction);
}
