/* Программа с асинхронным получением информации о
   статусе двух завершившихся порожденных процессов */

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Функция my_handler – обработчик сигнала SIGCHLD
void my_handler(int nsig) {
  int status;
  pid_t pid;

  /* Опрашиваем статус завершившегося процесса и
     одновременно узнаем его идентификатор */
  if ((pid = waitpid(-1, &status, 0)) < 0) {
    // Если возникла ошибка – сообщаем о ней и продолжаем работу
    printf("Some error on waitpid errno = %d\n", errno);
  } else {
    // Иначе анализируем статус завершившегося процесса
    if ((status & 0xff) == 0) { // 8 единиц
      // Процесс завершился с явным или неявным вызовом функции exit()
      printf("Process %d was exited with status %d\n", pid,
              status >> 8);
    } else if ((status & 0xff00) == 0) { // 8 единиц и 8 нулей
      // Процесс был завершен с помощью сигнала
      printf("Process %d killed by signal %d %s core file\n", pid,
              status & 0x7f, // 7 единиц
              (status & 0x80) ? "with" : "without"); // 1 и 7 нулей
    }
  }
}

int main() {
  // Устанавливаем обработчик для сигнала SIGCHLD
  signal(SIGCHLD, my_handler);

  // Порождаем Сhild 1
  pid_t pid = 0;
  if ((pid = fork()) < 0) {
    printf("Can't fork child 1\n");
    exit(1);
  } else if (pid == 0) {
    // Child 1 завершается с кодом 200
    exit(200);
  }
  
  // Продолжение процесса-родителя – порождаем Сhild 2
  if ((pid = fork()) < 0) {
    printf("Can't fork child 2\n");
    exit(1);
  } else if (pid == 0) {
    // Child 2 циклится, необходимо удалять с помощью сигнала!
    while (1);
  }

  // Продолжение процесса-родителя – уходим в цикл
  while (1);

  return 0;
}
