#ifndef NETWORK_H
#define NETWORK_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct Network Network;
typedef struct Layer Layer;

struct Layer
{
    float net;          // Unactivated outputs
    float out;          // Activated outputs
    float gradient;     // Gradients
    float *weights;     // Weights
    float bias;         // Bias
    float *lastWeights; // Previous weights
    float lastBias;     // Previous bias
};

struct Network
{
    Layer** inputs;     // Input layer array
    size_t NBI;         // Number of inputs (image w*h)

    Layer** hiddens;    // Hidden layer array
    size_t NBH;         // Number of nodes in the hidden layer

    Layer** outputs;    // Output layer array
    size_t NBO;         // Number of outputs (amount of characters recognized)
};

/* TRAIN */
// 1 - Create an empty struct Network
// 2 - Initialise it with the wanted parameters, it will train automatically
void createNetwork(Network *n, int NBO, int NBI, int NBH);

/* EVAL */
// 1 - Create an empty struct Network
// 2 - Load the existing network (path = "NeuralNetwork/save/network_save")
// 3 - Test the image
char network(SDL_Surface *img);

#endif
