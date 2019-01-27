all: mandelbrot.c
	mpicc -o mandelbrot -lm mandelbrot.c

clean:
	rm -rf ./mandelbrot

