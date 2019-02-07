Homework 1 SF2568
---
By Abgeiba Izunza and Sergio Liberman

Run the complete image from the Mandelbrot Algorithm:

mpirun -np \<procesors\> ./mandelbrot -w \<image width\> -h \<image hight\>

Run the program with magnification of a region:

mpirun -np \<procesors\> ./mandelbrot -w \<image width\> -h \<image hight\> -r \<y starting magnification point\> \<x starting magnification point\> \<y apperture size\> \<x apperture size\>

Example:
mpirun -np 47 ./mandelbrot -w 1500 -h 1500 -r 500 600 100 100
