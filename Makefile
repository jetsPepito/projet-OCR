CPPFLAGS = -MMD
CC = gcc
CPPFLAGS =  `pkg-config gtk+-2.0 --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =
LDLIBS= `pkg-config gtk+-2.0 --libs sdl` -lSDL_image -lm

SRC = ImageTreatment/image_treatment.c ImageTreatment/sdl.c BasicFunctions/basic.c GTK+/callback.c GTK+/error.c Segmentation/Histogram.c Segmentation/Segmentation.c NeuralNetwork/network.c GTK+/UI.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main main.d

-include ${DEP}
