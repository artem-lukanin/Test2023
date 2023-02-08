#include <fcntl.h>    // open
#include <stdio.h>    // printf
#include <unistd.h>   // write
#include <stdlib.h>   // exit
#include <sys/stat.h> // umask
#include <string.h>   // strlen

int main() {
  /* Обнуляем маску создания файлов текущего процесса для того, чтобы права доступа
  у создаваемого файла точно соответствовали параметру вызова open() */
  umask(0);
  
  int fd = 0;
  /* Попытаемся открыть файл с именем hello_file в текущей директории только для
  операций вывода. Если файла не существует, попробуем его создать с правами дос-
  тупа 0666, т. е. read-write для всех категорий пользователей */
  if ((fd = open("hello_file", O_WRONLY | O_CREAT, 0666)) < 0) {
    /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
    printf("Can't open file\n");
    exit(-1);
  }

  char string[] = "Hello, world!";
  /* Пробуем записать в файл строку "Hello, world!" вместе
  с признаком конца строки */
  size_t size = write(fd, string, strlen(string) + 1);
  if (size != strlen(string) + 1) {
    /* Если записалось меньшее количество байт, сообщаем об ошибке */
    printf("Can't write all string\n");
    exit(-1);
  }

  /* Закрываем файл */
  if (close(fd) < 0) {
    printf("Can't close file\n");
  }
  return 0;
}
