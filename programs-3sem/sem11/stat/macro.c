#include <stdio.h> 
#include <stdlib.h>

#define NUMBER 100
#define SUM(x, y) (x + y)

int sum(int x, int y) {
  return x + y;
}

int main(int argc, char *argv[]) {
  printf("%d\n", NUMBER);
  // printf("%d\n", 100);

  if (argc < 3) {
    printf("Not enough arguments\n");
    exit(-1);
  }

  float a = atof(argv[1]);
  float b = atof(argv[2]);

	printf("%f\n", SUM(a, b));
  // printf("%d\n", (a + b));

  return 0;
}

