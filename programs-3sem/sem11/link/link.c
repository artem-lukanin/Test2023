// Программа выводит список файлов в директории

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>    // exit
#include <unistd.h>    // symlink
#include <sys/stat.h>  // lstat
#include <string.h>

#define LINK_PREFIX "link_"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Not enough arguments\n");
    exit(-1);
  }

  char *filename = argv[1];

  size_t length = strlen(argv[1]) + strlen(LINK_PREFIX) + 1;
  char *link_filename = calloc(length, sizeof(char));
  strcpy(link_filename, LINK_PREFIX);
  strcat(link_filename, argv[1]);

  if (symlink(filename, link_filename) < 0) {
    printf("Failed to create symlink\n");
    exit(-1);
  }

  printf("Created symlink %s\n", link_filename);

  struct stat buf;
  if (lstat(link_filename, &buf) < 0) {
    printf("Failed to get stat\n");
    exit(-1);
  }

  printf("---------\n");
  printf("Size: %ld\n", buf.st_size);
  printf("UID: %d\n", buf.st_uid);
  printf("GID: %d\n", buf.st_gid);

  free(link_filename);

  return 0;
}

