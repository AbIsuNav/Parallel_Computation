#include <stdio.h>

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

int main(int argc, char *argv) {
  unsigned char *demo;

  demo = (unsigned char *)"Hello world!\n";
  save_ascii(demo, 14, 1);
}


