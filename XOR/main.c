#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*Declare the functions so main stays on top*/
double sig(double x);
double predict(int i1, int i2, int target, double** w1, double* w2);
void train(int n, double** w1, double* w2, int argc);
void test(int i1, int i2, double** w1, double* w2);

//NBINPUTS (i): 2 + bias (3 out)
//NBHIDDEN (h): 2 + bias (2 net, 3 out)
//NBOUTPU  (-): 1		 (1 net, 1 out)

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

	//DEBUG:
	//print("%li\n", t);
	//unsigned long int t = 1539718702;
	//srand(t);

	/*Declare weigths*/
	double **w1;	//w1[h (excl. bias)][input (incl. biases)]
	w1 = (double **) malloc(2*sizeof(double *));
	for(int h = 0; h < 2; h++)
		w1[h] = (double *) malloc(3*sizeof(double));

	double *w2;		//w2[h (incl. bias)]
	w2 = (double *) malloc(3*sizeof(double));

	/*Initialize weigths with random values from 0 to 1*/
	for(int h = 0; h < 2; h++)
	{
		w1[h][0] = 0.0;
		for(int i = 1; i < 3; i++)
		{
			w1[h][i] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
		}
	}
	w2[0] = 0.0;
	for(int h = 1; h < 3; h++)
	{
		w2[h] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
	}

	/*Train and test the result*/
	printf("\nTraining...\n");
	train(1000000, w1, w2, argc);

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


/*Neural network*/ //targ = -1 -> no backprop
double predict(int i1, int i2, int target, double** w1, double* w2)
{
	/*Initialize inputs and variables*/
	double inputs[3] = {-1.0, ((i1 == 1) ? 1.0 : 0.0), ((i2 == 1) ? 1.0 : 0.0)};
	double hnet[2] = {0.0, 0.0};
	double hout[3] = {-1.0, 0.0, 0.0};
	double targ = (target == 1) ? 1.0 : 0.0;

	/*Forward propagation*/
	//Inputs -> Hidden layer
	for(int h = 0; h < 2; h++)
	{
		for(int i = 0; i < 3; i++)
		{
			hnet[h] += inputs[i] * w1[h][i];
		}
	}

	for(int h = 0; h < 2; h++)
		hout[h+1] = sig(hnet[h]);

	//Hidden layer -> Output
	double net = 0;
	for(int h = 0; h < 3; h++)
		net += hout[h] * w2[h];
	double out = sig(net);

	/*Backward propagation*/
	if(target != -1)
	{
		//Output -> Hidden Layer
		for(int h = 0; h < 3; h++)
		{
			double delta = (out - targ) * (out * (1.0 - out)) * hout[h];
			w2[h] -= delta * 0.7;
		}

		//Hidden layer -> Inputs
		for(int h = 0; h < 2; h++)
		{
			for(int i = 0; i < 3; i++)
			{
				double delta = (out - targ) * (out * (1.0 - out)) * w2[h+1]
					* (hout[h+1] * (1 - hout[h+1])) * inputs[i];
				w1[h][i] -= delta * 0.7;
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
		if(argc <= 1)
		{
			if(i <= 5)
				printf("{%i,%i} : %g\n", i1, i2, prediction);
			if(i == 6)
				printf("    (...)\n");
			if(i >= n - 6)
				printf("{%i,%i} : %g\n", i1, i2, prediction);
		}
	}
}


/*Evaluation (without backpropagation*/
void test(int i1, int i2, double** w1, double* w2)
{
	double prediction = predict(i1, i2, -1, w1, w2);
	if (prediction >= 0.9)
		printf("%i XOR %i = 1 	(prediction: %g)\n", i1, i2, prediction);
	else if (prediction <= 0.1)
		printf("%i XOR %i = 0 	(prediction: %g)\n", i1, i2, prediction);
	else
		printf("TOO INNACURATE, TRAIN MORE (%g)\n", prediction);
}
