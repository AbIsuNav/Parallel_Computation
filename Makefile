optimized: mandelbrot.c	
	mpicc -o mandelbrot -lm mandelbrot.c -Wall -O3
debug: mandelbrot.c	
	mpicc -o mandelbrot -lm mandelbrot.c -Wall -g
not_optimized: mandelbrot.c
	mpicc -o mandelbrot -lm mandelbrot.c -Wall

clean:
	rm -rf ./mandelbrot

