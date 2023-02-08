#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main() {
  int a = 1;

  pid_t res = fork();

  if (res < 0) {
    // Ошибка создания дочернего процесса
    printf("Failed to create a new process!\n");
  } else if (res > 0) {
    // Находимся в процессе-родителе
    a += 10;
    sleep(1);
  } else {
    // Находимся в процессе-ребенке
    a++;
  }

  // Находимся в обоих процессах
  printf("PID: %d, PPID: %d, a = %d\n",
         getpid(), getppid(), a);

  return 0;
}
