#Define the default target

all: main

#Define dependencies and compile information.

main: main.c
	gcc main.c `pkg-config --cflags --libs gtk+-2.0` -o main

main.c: callback.c document.h
	gcc -c callback.c




