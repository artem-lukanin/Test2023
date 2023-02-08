#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_CHAR 10000

int main() {
  int fd;

  if ((fd = open("file2.c", O_RDONLY)) < 0) {
    printf("Can't open file\n");
    exit(-1);
  }

  char string[NUM_CHAR];
  size_t size = read(fd, string, NUM_CHAR);
  if (size < 0) {
    printf("Can't read file\n");
    exit(-1);
  }

  printf("Number of characters: %d\n", (int)size);

  printf("%s\n", string);

  if (close(fd) < 0) {
    printf("Can't close file\n");
  }
  return 0;
}
