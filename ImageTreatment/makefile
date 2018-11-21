## Simple SDL mini code
CPPFLAGS= -MMD
CC=gcc
CFLAGS= -Wall -Werror -Wextra -std=c99  `pkg-config --cflags sdl`
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl` -lSDL_image -lm

SRC= main.c image_treatment.c ../BasicFunctions/basic.c sdl.c
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
