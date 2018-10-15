#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*Declare the functions so main stays on top*/
double sig(double x);
double predict(int i1, int i2, int targ);
void train(int n);
void test(int i1, int i2);


int argc, const char* argv[]{	
	/*Initialize RNG*/	
	time_t t;
	srand((unsigned) time(&t));
	
	
	/*Initialize constant values*/	
	const int NBI = 4;			//number of inputs including biases
	const int NBH = 3;			//number of nodes in the hidden layer including bias
	const double ETA = 0.20;	//learning const
	
	
	/*Declare weigths*/	
	double w1[NBH - 1][NBI];	//w1[h (excl. bias)][input (incl. biases)]
	double w2[NBH];				//w2[h (incl. bias)]
	
	/*Initialize weigths with random values from 0 to 1*/
	for(int h = 0; h < NBH - 1; h++){
		for(int i = 0; i < NBI; i++){
			w1[h][i] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
		}
	}		
	for(int i = 0; h < NBH; h++){
		w2[h] = (double)(rand() % (100 + 1 - 0) + 0) / (double)(100.0);
	}
	
	/*Train and test the result*/
	train(50000);
	test(0, 0);
	test(0, 1);
	test(1, 0);
	test(1, 1);
	
	/*I am very confident with my code*/
	return EXIT_SUCCESS;
}


/*Sigmoid function*/
double sig(double x){
	return (exp(x) / (exp(x) + 1));
}


/*Neural network*/
double predict(int i1, int i2, int target){	//targ = -1 -> no backpropagation
	/*Initialize inputs and variables*/
	double inputs[NBI] = {1.0, 1.0, ((i1 == 1) ? 1.0 : 0.0), ((i2 == 1) ? 1.0 : 0.0)};
	double hnet[NBH - 1] = {0.0, 0.0};
	double hout[NBH] = {1.0, 0.0, 0.0};
	double targ = (target == 1) ? 1.0 : 0.0;
	
	/*Forward propagation*/
	//Inputs -> Hidden layer
	for(int h = 0; h < NBH - 1; h++){
		for(int i = 0; i < NBI: i++){
			hnet[h] += inputs[i] * w1[h][i];
		}
	}
	//hnet[0] = inputs[0] * w1[0][0] +  inputs[1] * w1[0][1] + inputs[2] * w1[0][2] + inputs[3] * w1[0][3];
	//hnet[1] = inputs[0] * w1[1][0] +  inputs[1] * w1[1][1] + inputs[2] * w1[1][2] + inputs[3] * w1[1][3];
	for(int h = 0; h < NBH - 1; h++){hout[h+1] = sig(hnet[h]);}
	
	//Hidden layer -> Output
	double net = 0;
	for(int h = 0; h < NBH; h++){net += hout[h] * w2[h];}
	double out = sig(net);
	
	/*Backward propagation*/
	if(target != -1){
		//Output -> Hidden Layer
		for(int h = 0; h < NBH; h++){
			double delta = (out - targ) * (out * (1 - out)) * hout[h];
			w2[h] -= delta * ETA;
		}
	
		//Hidden layer -> Inputs
		for(int h = 0; h < NBH - 1; h++){
			for(int i = 0; i < NBI; i++){
				double delta = (out - targ) * (out * (1 - out)) * w2[h+1] * (hout[h+1] * (1 - hout[h+1])) * inputs[i];
				w1[h][i] -= delta * ETA;
			}
		}
	}
	
	return out;
}


/*Training loop (with backpropagation)*/
void train(int n){
	for(int i = 0; i < n; i++){
		int i1 = rand() % (1 + 1 - 0) + 0;
		int i2 = rand() % (1 + 1 - 0) + 0;		
		int targ = (i1 == i2) ? 0 : 1;
		
		prediction = predict(i1, i2, targ);
		
		/*Purely for showcase, remove to optimize*/
		if(i <= 10){printf("{%i,%i} : %g\n", i1, i2, prediction);}
		if(i == 11){printf"...\n");}
		if(i >= n - 11){printf("{%i,%i} : %g\n", i1, i2, prediction);}
	}
}


/*Evaluation (without backpropagation*/
void test(int i1, int i2){
	printf("\n\n\n");
	double prediction = predict(i1, i2, -1);
	if (prediction >= 0.9){printf("%i XOR %i = 1 (exact: %g\n", i1, i2, prediction);}
	else if (prediction <= 0.1){printf("%i XOR %i = 0 (exact: %g\n", i1, i2, prediction);}
	else {printf("TOO INNACURATE, TRAIN MORE");}
}
