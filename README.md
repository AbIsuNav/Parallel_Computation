Homework 1 SF2568
---
By Abgeiba Izunza and Sergio Liberman

Run the complete image from the Mandelbrot Algorithm:

mpirun -np \<procesors\> ./mandelbrot -w \<image width\> -h \<image hight\>

Run the program with magnification of a region:

mpirun -np \<procesors\> ./mandelbrot -w \<image width\> -h \<image hight\> -r \<y (columns) starting magnification point\> \<x (rows) starting magnification point\> \<y (columns) zooming area size\> \<x (rows) zooming area size\>

Example:
mpirun -np 47 ./mandelbrot -w 1500 -h 1400 -r 500 600 100 100
would produce a 1500x1400 image, which corresponds to the zoomed area of the original defined by vertexes (500, 600) and (500+100, 600+100)=(600, 700)
