## Simple SDL mini code
CPPFLAGS= -MMD
CC=gcc
CFLAGS= -Wall -Werror -Wextra -std=c99  `pkg-config --cflags sdl` -g
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl` -lSDL_image -lm

SRC= train.c network.c ../BasicFunctions/basic.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

train: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} train

-include ${DEP}

# END
