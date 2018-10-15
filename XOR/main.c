#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Declare the functions so main stays on top*/
double sig(double x);
double predict(int i1, int i2, int target, double** w1, double* w2);
void train(int n, double** w1, double* w2);
void test(int i1, int i2, double** w1, double* w2);


int main(/*int argc, const char* argv[]*/)
{
	/*Initialize RNG*/
	time_t t;
	srand((unsigned) time(&t));

	/*Declare weigths*/
	double w1[2][3];	//w1[h (excl. bias)][input (incl. biases)]
	double w2[3];		//w2[h (incl. bias)]

	/*Initialize weigths with random values from 0 to 1*/
	for(int h = 0; h < 2; h++)
	{
		for(int i = 0; i < 3; i++)
		{
			w1[h][i] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
		}
	}
	for(int h = 0; h < 3; h++)
	{
		w2[h] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
	}

	/*Train and test the result*/
	train(50000, (double **)w1, (double *)w2);
	test(0, 0, (double **)w1, (double *)w2);
	test(0, 1, (double **)w1, (double *)w2);
	test(1, 0, (double **)w1, (double *)w2);
	test(1, 1, (double **)w1, (double *)w2);

	/*I am very confident with my code*/
	return EXIT_SUCCESS;
}


/*Sigmoid function*/
double sig(double x)
{
	return ((exp(x)) / (exp(x) + 1));
}


/*Neural network*/ //targ = -1 -> no backprop
double predict(int i1, int i2, int target, double** w1, double* w2)
{
	/*Initialize inputs and variables*/
	double inputs[3] = {1.0, ((i1 == 1) ? 1.0 : 0.0), ((i2 == 1) ? 1.0 : 0.0)};
	double hnet[2] = {0.0, 0.0};
	double hout[3] = {1.0, 0.0, 0.0};
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
	//In case it doesn't work like in Python:
	/*
	hnet[0] = inputs[0] * w1[0][0] +  inputs[1] * w1[0][1] + inputs[2]
		* w1[0][2] + inputs[3] * w1[0][3];
	hnet[1] = inputs[0] * w1[1][0] +  inputs[1] * w1[1][1] + inputs[2]
		* w1[1][2] + inputs[3] * w1[1][3];
	*/
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
			double delta = (out - targ) * (out * (1 - out)) * hout[h];
			w2[h] -= delta * 0.2;
		}

		//Hidden layer -> Inputs
		for(int h = 0; h < 2; h++)
		{
			for(int i = 0; i < 3; i++)
			{
				double delta = (out - targ) * (out * (1 - out)) * w2[h+1] * (hout[h+1] * (1 - hout[h+1])) * inputs[i];
				w1[h][i] -= delta * 0.2;
			}
		}
	}

	return out;
}


/*Training loop (with backpropagation)*/
void train(int n, double** w1, double* w2)
{
	for(int i = 0; i < n; i++)
	{
		int i1 = rand() % (1 + 1 - 0) + 0;
		int i2 = rand() % (1 + 1 - 0) + 0;
		int targ = (i1 == i2) ? 0 : 1;

		double prediction = predict(i1, i2, targ, w1, w2);

		/*Purely for showcase, remove to optimize*/
		if(i <= 10)
			printf("{%i,%i} : %g\n", i1, i2, prediction);
		if(i == 11)
			printf("...\n");
		if(i >= n - 11)
			printf("{%i,%i} : %g\n", i1, i2, prediction);
	}
}


/*Evaluation (without backpropagation*/
void test(int i1, int i2, double** w1, double* w2)
{
	printf("\n\n\n");
	double prediction = predict(i1, i2, -1, w1, w2);
	if (prediction >= 0.9)
		printf("%i XOR %i = 1 (exact: %g\n", i1, i2, prediction);
	else if (prediction <= 0.1)
		printf("%i XOR %i = 0 (exact: %g\n", i1, i2, prediction);
	else
		printf("TOO INNACURATE, TRAIN MORE");
}
