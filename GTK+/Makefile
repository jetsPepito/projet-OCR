# Makefile

CPPFLAGS= `pkg-config --cflags gtk+-2.0 sdl` -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
LDLIBS = -lm `pkg-config --libs gtk+-2.0 sdl` -lSDL_image

all: main

SRC = main.c callback.c ../ImageTreatement/image_treatement.h ../BasicFunctions/basic.h ../ImageTreatment/sdl.h ../Segmentation/Segmentation.h ../Segmentation/Histogram.h ../NeuralNetwork/network.h
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}


-include ${DEP}

# END



#all: main
#
#main: main.c
#	gcc main.c `pkg-config --cflags --libs gtk+-2.0 sdl` -lSDL_image -lm -o main
#
#main.c: callback.c document.h ../ImageTreatment/image_treatment.c
#	gcc -c callback.c ../ImageTreatment/image_treatment.c
