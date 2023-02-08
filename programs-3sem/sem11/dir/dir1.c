// Программа выводит список файлов в директории

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>    // *dir
#include <stdlib.h>    // exit

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Not enough arguments\n");
    exit(-1);
  }

  DIR *dir = opendir(argv[1]);
  if (dir == NULL) {
    printf("Can't open a directory\n");
    exit(-1);
  }

  printf("List of files in the folder \"%s\":\n", argv[1]);

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  if (closedir(dir) < 0) {
    printf("Can't close a directory\n");
    exit(-1);
  }

  return 0;
}

