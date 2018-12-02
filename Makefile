CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `pkg-config --cflags sdl`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl`

SRC = main.c ImageTreatment/image_treatment.c ImageTreatment/sdl.c Segmentation/ Neural_Netork/Network.c Neural_Netork/saveNetwork.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

-include ${DEP}
