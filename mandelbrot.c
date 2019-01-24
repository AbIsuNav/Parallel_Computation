#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <complex.h>


void save_ascii(unsigned char *color, int N, int M) {
  FILE *fp;

  fp = fopen("color.txt", "w");
  for (int j = 0; j < N; j++) {
    for (int i = 0; i < M; i++) {
      fprintf(fp, "%hhu ", color[i + j*M]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

void master() {

}

unsigned char cal_pixel(double complex d, double b, int N) {
  int count = 1;
  double complex z = 0.0;
  while ((cabs(z) < b) && (count < N)) {
    z = cpow(z, 2.0 + I * 0.0) + d;
    count++;
  }
  return count;
}

void slave(int p, int q, int hp, int wp, double dy, double dx, int b, int N, int P, int Q) {
  unsigned char buf[P * Q];
  double dreal, dimag;
  double complex d;
  int xoff = p * wp;
  int yoff = q * hp; 
  for (int x = p * hp; x < (p + 1) * hp; x++) {
    dreal = (x + xoff) * dx - b;
    for (int y = q * wp; y < (q + 1) * wp; y++) {
      dimag = (y + yoff) * dy - b;
      d = dreal + dimag * I;
      buf[x * wp + y] = cal_pixel(d, b, N);
    }
  }
}

int main(int argc, char *argv[]) {
  int myrank;
  int w = 2048;
  int h = 2048;
  int P = 1;
  int Q = 4;
  int N = 256;
  double b = 1;
  
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  double dx = 2.0 * b / (w - 1);
  double dy = 2.0 * b / (h - 1);
  int wp = w / P;
  int hp = h / P;
  int yoff = 0;
  double dreal, dimag;
  double complex d;
  unsigned char color[w*h];
  
  for (int x = 0; x < wp; x++) {
    dreal = (x + xoff) * dx - b;
    /* dreal = x * dx - b; */
    for (int y = 0; y < h; y++) {
      dimag = (y + yoff) * dy - b;
      /* dimag = y * dy - b; */
      d = dreal + dimag * I;
      color[x * w + y] = cal_pixel(d, b, N);
    }
  }
  save_ascii(color, w, h);
  /* switch (myrank) { */
  /* case 0: */
  /*   master(); */
  /*   break; */
  /* case 1: */
  /*   slave(); */
  /*   break; */
  /* default: */
  /*   printf("Error: rank is invalid\n"); */
  /*   break; */
  /* } */
  
  //MPI_Finalize();
}


