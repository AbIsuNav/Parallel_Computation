#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <complex.h>
#include <unistd.h>

#define TAG 123

void save_ascii(unsigned char *color, int N, int M) {
  FILE *fp;
  int i, j;

  fp = fopen("color.txt", "w");
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      fprintf(fp, "%hhu ", color[j + i*N]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

void master(int w, int h, int hp, int numprocs) {
  MPI_Status status;

  int q;
  unsigned char color[w * h];
  int ioff;
  int rc;
  for (q = 1; q < numprocs; q++) {
    ioff = (q - 1) * hp;
    rc = MPI_Recv((void *)(color + ioff * w), w * hp, MPI_UNSIGNED_CHAR, q, TAG, MPI_COMM_WORLD, &status);
    printf("Return code from recv/rank %d: %d\n", q, rc);
  }
  save_ascii(color, h, w);
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

void slave(int p, int q, int hp, int w, double dy, double dx, int b, int N, double rx, double ry, double rh, double rw) {
  unsigned char buf[w * hp];
  double dreal, dimag;
  double complex d;
  double xoff = q * rh;
  double yoff = 0.0;
  double x_step = rh / hp;
  double y_step = rw / w;
  int ix, iy;
  for (ix = 0; ix < hp; ix++) {
    dreal = (rx + ix*x_step + xoff) * dx - b;
    for (iy = 0; iy < w; iy++) {
      dimag = (ry + iy*y_step + yoff) * dy - b;
      d = dreal + dimag * I;
      buf[ix * w + iy] = cal_pixel(d, b, N);
    }
   }
  MPI_Send((void *)buf, w * hp, MPI_UNSIGNED_CHAR, 0, TAG, MPI_COMM_WORLD);
  /* printf("Return code from send/rank %d: %d\n", q+1, rc); */
  /* printf("Everything sent from rank %d\n", q + 1); */
}

int main(int argc, char *argv[]) {
  int myrank, numprocs;

  int i = 0;
  int w = 2048;
  int h = 2048;
  while (i < argc) {
    if (strcmp(argv[i], "-w") == 0) {
      w = atoi(argv[i+1]);
      i += 1;
    } else if (strcmp(argv[i], "-h") == 0) {
      h = atoi(argv[i+1]);
      i += 1;
    }
    i++;
  }

  int N = 256;
  double b = 1;
  double rx = 0, ry = 0, rw = w, rh = h;

  i = 0;
  while (i < argc) {
    if (strcmp(argv[i], "-r") == 0) {
      rx = atof(argv[i+1]);
      ry = atof(argv[i+2]);
      rh = atof(argv[i+3]);
      rw = atof(argv[i+4]);
      i += 4;
    }
    i++;
  }

  i = 1;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  double dy = 2.0 * b / (w - 1);
  double dx = 2.0 * b / (h - 1);
  int hp = h / (numprocs - 1);
  double rhp = rh / (numprocs - 1);
  
  printf("My rank is %d\n", myrank);
  switch (myrank) {
  case 0:
    master(w, h, hp, numprocs);
    break;
  default:
    slave(0, myrank - 1, hp, w, dy, dx, b, N, rx, ry, rhp, rw);
    break;
  }

  MPI_Finalize();
}
