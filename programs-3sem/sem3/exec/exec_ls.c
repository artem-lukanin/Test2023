#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Directory is not specified!\n");
    exit(-1);
  }

  int res = execl("/bin/ls", "/bin/ls", argv[1], (char *)NULL);
  if (res < 0) {
    printf("Failed to list home directory!\n");
    exit(-1);
  }
  return 0;
}
