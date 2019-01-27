all: mandelbrot.c
	mpicc -o mandelbrot -lm mandelbrot.c

