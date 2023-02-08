#include <unistd.h> // pipe, read, write, fork
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <string.h> // strlen

int main() {
  int fd[2];
  /* Попытаемся создать pipe */
  if (pipe(fd) < 0) {
    /* Если создать pipe не удалось, печатаем об
    этом сообщение и прекращаем работу */
    printf("Can't create a pipe\n");
    exit(-1);
  }

  char string[] = "Hello, world!";
  size_t string_size = strlen(string) + 1;
  char output_string[string_size];

  /* Порождаем новый процесс */
  pid_t res = fork();
  if (res == -1) {
    /* Если создать процесс не удалось, сообщаем
       об этом и завершаем работу */
    printf("Can't create a new process\n");
    exit(-1);
  } else if (res > 0) {
    /* Мы находимся в родительском процессе, который
       будет передавать информацию процессу-ребенку. В
       этом процессе выходной поток данных нам
       не понадобится, поэтому закрываем его. */
    close(fd[0]);

    /* Пробуем записать в pipe из нашего
    массива строку "Hello, world!" вместе
    с признаком конца строки */
    size_t size = write(fd[1], string, string_size);
    if (size != string_size) {
      /* Если записалось меньшее количество байт,
      сообщаем об ошибке */
      printf("Can't write all string\n");
      exit(-1);
    }

    /* Закрываем выходной поток*/
    if (close(fd[1]) < 0) {
      printf("Can't close output stream\n");
    }
    printf("End of parent process\n");
  } else {
    /* Мы находимся в порожденном процессе, который
       будет получать информацию от процесса-родителя.
       Он унаследовал от родителя таблицу открытых файлов
       и, зная файловые дескрипторы, соответствующие
       pip'у, может его использовать. В этом процессе
       входной поток данных нам не понадобится, поэтому
       закрываем его. */
    close(fd[1]);

    /* Пробуем прочитать из pip'а всю записанную строку
       в другой массив */
    size_t size = read(fd[0], output_string, string_size);
    if (size < 0) {
      /* Если прочитать не смогли, сообщаем об ошибке */
      printf("Can't read the string\n");
      exit(-1);
    }

    /* Закрываем входной поток*/
    if (close(fd[0]) < 0) {
      printf("Can't close input stream\n");
    }

    /* Печатаем прочитанную строку */
    printf("%s\n", output_string);
  }

  return 0;
}
