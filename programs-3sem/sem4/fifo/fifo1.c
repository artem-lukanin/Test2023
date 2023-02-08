#include <fcntl.h>    // open
#include <unistd.h>   // read, write, fork
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
       этом процессе открываем FIFO на запись */
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

    /* Закрываем входной поток данных и на этом
       родитель прекращает работу */
    if (close(fd) < 0) {
      printf("Can't close FIFO\n");
    }
    printf("End of parent process\n");
  } else {
    /* Мы находимся в порожденном процессе, который
       будет получать информацию от процесса-родителя.
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

    /* Закрываем входной поток и завершаем работу */
    if (close(fd) < 0) {
      printf("Can't close stream\n");
    }

    /* Печатаем прочитанную строку */
    printf("%s\n", output_string);
  }

  return 0;
}
