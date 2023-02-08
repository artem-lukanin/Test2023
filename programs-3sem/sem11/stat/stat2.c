/* Программа получает в качестве аргумента имя файла
   и выводит значения следующих атрибутов для файла:
   размер файла, UID, GID владельца,
   тип файла: регулярный файл или директория.
   Если аргумент не задан, создать файл и записать в него строку.
   Воспользоваться системным вызовом fstat() */

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILENAME "newfile.txt"
#define DEFAULT_TEXT "This is a new file text"

void print_file_attributes(struct stat buf) {
  printf("---------\n");
  printf("Size: %ld\n", buf.st_size);
  printf("UID: %d\n", buf.st_uid);
  printf("GID: %d\n", buf.st_gid);
  if (S_ISREG(buf.st_mode))
    printf("Type: Reg.file\n");
  else if (S_ISDIR(buf.st_mode))
    printf("Type: Directory\n");
}


int main(int argc, char *argv[]) {
  int fd = 0;

  if (argc < 2) {
    fd = creat(DEFAULT_FILENAME, 0666);
    char content[] = DEFAULT_TEXT;
    write(fd, content, strlen(content) + 1);
  } else {
    fd = open(argv[1], O_RDONLY);
  }
  
  struct stat buf;
  if (fstat(fd, &buf) < 0) {
    printf("Could not read file attributes\n");
    exit(-1);
  }
  close(fd);

  print_file_attributes(buf);

  return 0;
}

