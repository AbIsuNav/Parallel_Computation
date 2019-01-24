#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <complex.h>

#define TAG 123

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

void master(int w, int h, int hp, int numprocs) {
  MPI_Status status;
  
  unsigned char color[w * h];
  int ioff;
  for (int q = 1; q < numprocs; q++) {
    ioff = (q - 1) * hp;
    int rc = MPI_Recv((void *)(color + ioff * w), w * hp, MPI_UNSIGNED_CHAR, q, TAG, MPI_COMM_WORLD, &status);
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

void slave(int p, int q, int hp, int w, double dy, double dx, int b, int N) {
  unsigned char buf[w * hp];
  double dreal, dimag;
  double complex d;
  int xoff = q * hp;
  int yoff = 0;
  for (int x = 0; x < hp; x++) {
    dreal = (x + xoff) * dx - b;
    for (int y = 0; y < w; y++) {
      dimag = (y + yoff) * dy - b;
      d = dreal + dimag * I;
      buf[x * w + y] = cal_pixel(d, b, N);
    }
  }
  int rc = MPI_Send((void *)buf, w * hp, MPI_UNSIGNED_CHAR, 0, TAG, MPI_COMM_WORLD);
  /* printf("Return code from send/rank %d: %d\n", q+1, rc); */
  /* printf("Everything sent from rank %d\n", q + 1); */
}

int main(int argc, char *argv[]) {
  int myrank, numprocs;
  int w = 2048;
  int h = 2048;
  int N = 256;
  double b = 1; 
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  double dy = 2.0 * b / (w - 1);
  double dx = 2.0 * b / (h - 1);
  int hp = h / (numprocs - 1);
  
  printf("My rank is %d\n", myrank);
  switch (myrank) {
  case 0:
    master(w, h, hp, numprocs);
    break;
  default:
    slave(0, myrank - 1, hp, w, dy, dx, b, N);
    break;
  }

  MPI_Finalize();
}


