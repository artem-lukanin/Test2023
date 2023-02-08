#include <fcntl.h>    // open
#include <unistd.h>   // read
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // strlen

int main() {
  char fifo_file_name[] = "fifo_file";

  int fd = 0;
  char string[] = "Hello, world!";
  size_t string_size = strlen(string) + 1;
  char output_string[string_size];

  /* Данный процесс будет получать информацию из FIFO.
     Открываем FIFO на чтение. */
  if ((fd = open(fifo_file_name, O_RDONLY)) < 0) {
    /* Если открыть FIFO не удалось, печатаем об
       этом сообщение и прекращаем работу */
    printf("Can't open FIFO for reading\n");
    exit(-1);
  }

  /* Пробуем прочитать из FIFO всю записанную строку
     в другой массив */
  size_t size = read(fd, output_string, string_size);
  if (size < 0) {
    /* Если прочитать не смогли, сообщаем об ошибке */
    printf("Can't read the string\n");
    exit(-1);
  }

  /* Закрываем входной поток */
  if (close(fd) < 0) {
    printf("Can't close stream\n");
  }

  /* Печатаем прочитанную строку */
  printf("%s\n", output_string);

  return 0;
}
