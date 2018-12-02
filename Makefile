CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `pkg-config --cflags sdl`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl`

SRC =  ImageTreatment/image_treatment.c ImageTreatment/sdl.c BasicFunctions/basic.c GTK+/callback.c GTK+/error.c Segmentation/Histogram.c Segmentation/Segmentation.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

-include ${DEP}
