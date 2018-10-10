## Simple SDL mini code
CPPFLAGS= -MMD
CC=gcc
CFLAGS= -Wall -Wextra -std=c99  `pkg-config --cflags sdl`
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl`

SRC= main.c
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
