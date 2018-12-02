# Makefile

CPPFLAGS = `pkg-config --cflags gtk+-2.0 sdl` -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
LDLIBS = -lm `pkg-config --libs gtk+-2.0 sdl` -lSDL_image

all: main

SRC = main.c callback.c ../ImageTreatment/image_treatment.c ../BasicFunctions/basic.c ../ImageTreatment/sdl.c ../Segmentation/Segmentation.c ../Segmentation/Histogram.c ../NeuralNetwork/network.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}


-include ${DEP}

# END



