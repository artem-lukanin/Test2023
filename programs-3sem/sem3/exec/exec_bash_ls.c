#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // Для составных команд используем bash -c "<команда>"
  int res = execl("/bin/bash", "/bin/bash", "-c",
                  "ls /home/user > ls.txt", (char *)NULL);
  if (res < 0) {
    printf("Failed to save home directory content to file!\n");
    exit(-1);
  }
  return 0;
}
