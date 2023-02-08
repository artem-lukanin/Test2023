#include <stdio.h>

int main(int argc, char* argv[], char *envp[]) {
  int i = 0;

  printf("ARGS:\n");
  // Выводим аргументы командной строки
  for (i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }

  printf("ENVS:\n");
  i = 0;
  // Выводим переменные окружающей среды текущего процесса
  while (envp[i] != NULL) {
    printf("%s\n", envp[i++]);
  }

  return 0;
}
