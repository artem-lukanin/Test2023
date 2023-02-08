#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int fd;
  if ((fd = open("file.txt", O_RDONLY)) < 0) {
    printf("Can't open a file\n");
    exit(-1);
  }

  off_t offset1 = lseek(fd, 5, SEEK_SET);
  off_t offset2 = lseek(fd, 3, SEEK_CUR);
  off_t offset3 = lseek(fd, 0, SEEK_END);

  close(fd);

  printf("%ld %ld %ld\n", offset1, offset2, offset3);

  return 0;
}

