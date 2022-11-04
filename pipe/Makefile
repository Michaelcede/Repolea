all: build build/time


build:
		mkdir -p build

build/time: timepipe.c
		gcc -g -Wall -o build/time timepipe.c

clean:
		rm build/*
