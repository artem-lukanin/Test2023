#include <fcntl.h>    // open
#include <unistd.h>   // write
#include <sys/stat.h> // umask, mknod
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // strlen

int main() {
  /* Обнуляем маску создания файлов текущего процесса
     для того, чтобы права доступа у создаваемого FIFO
     точно соответствовали параметру вызова mknod() */
  umask(0);

  char fifo_file_name[] = "fifo_file";

  /* Попытаемся создать FIFO с именем fifo_file в
     текущей директории */
  if (mknod(fifo_file_name, S_IFIFO | 0666, 0) < 0) {
  //if (mkfifo(fifo_file_name, 0666) < 0) {
    /* Если создать FIFO не удалось, печатаем об
       этом сообщение и прекращаем работу */
    printf("Can't create FIFO\n");
    exit(-1);
  }

  int fd = 0;
  char string[] = "Hello, world!";
  size_t string_size = strlen(string) + 1;

  /* Данный процесс будет записывать
     информацию в FIFO. Открываем FIFO на запись */
  if ((fd = open(fifo_file_name, O_WRONLY)) < 0) {
    /* Если открыть FIFO не удалось, печатаем
       об этом сообщение и прекращаем работу */
    printf("Can't open FIFO for writing\n");
    exit(-1);
  }

  /* Пробуем записать в FIFO из нашего
  массива строку "Hello, world!" вместе
  с признаком конца строки */
  size_t size = write(fd, string, string_size);
  if (size != string_size) {
    /* Если записалось меньшее количество байт,
    сообщаем об ошибке */
    printf("Can't write all string\n");
    exit(-1);
  }

  /* Закрываем входной поток данных */
  if (close(fd) < 0) {
    printf("Can't close FIFO\n");
  }

  return 0;
}
