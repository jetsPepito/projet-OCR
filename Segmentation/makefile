##Simple SDL mini code
CPPFLAGS= -MMD
CC=gcc
CFLAGS= -g -Wall -Wextra -std=c99  `pkg-config --cflags sdl`
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl` -lSDL_image -lm

SRC= main.c Segmentation.c Histogram.c ../BasicFunctions/basic.c ../ImageTreatment/image_treatment.c ../ImageTreatment/sdl.c
OBJ= ${SRC:.c=.o}
	DEP= ${SRC:.c=.d}

main: ${OBJ}

.PHONY: clean

clean:
		${RM} ${OBJ}
		${RM} ${DEP}
		${RM} main

-include ${DEP}

# END
