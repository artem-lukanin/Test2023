#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t res = fork();

  if (res == -1) {
    // Обработка ошибки создания процесса
    printf("Failed to create a new process!\n");
    exit(-1);
  } else if (res > 0) {
    // Компилируем программу в родительском процессе
    execl("/usr/bin/gcc", "/usr/bin/gcc",
          "hello.c", "-o", "hello.o", (char *)NULL);
    printf("Failed to compile a program!\n");
  } else if (res == 0) {
    // В дочернем процессе ждем и запускаем программу
    sleep(1);
    execl("./hello.o", "./hello.o", (char *) NULL);
    printf("Failed to run a program!\n");
  }

  return 0;
}
