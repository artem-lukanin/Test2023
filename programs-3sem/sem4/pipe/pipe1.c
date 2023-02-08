#include <unistd.h> // pipe, read, write
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

  /* Пробуем прочитать из pip'а всю записанную строку
     в другой массив */
  size = read(fd[0], output_string, string_size);
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

  return 0;
}
