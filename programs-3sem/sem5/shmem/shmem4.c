#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <sys/types.h> // key_t
#include <sys/ipc.h>   // ftok, shm*
#include <sys/shm.h>   // shm*

#define CHAR_NUMBER 10000

int main() {
  char *str; /* Указатель на разделяемую память */

  int shmid; /* IPC дескриптор для области
                емой памяти */
  /* Имя файла, используемое для генерации ключа.
     Файл с таким именем должен существовать
     в текущей директории */
  char pathname[] = "shmem3.c";

  key_t key; /* IPC ключ */
  /* Генерируем IPC ключ из
     имени файла shmem3.c в текущей директории
     и номера экземпляра области разделяемой памяти 0 */
  if ((key = ftok(pathname, 0)) < 0) {
      printf("Can't generate a key\n");
      exit(-1);
  }

  /* Пытаемся получить IPC дескриптор созданного сегмента
     разделяемой памяти для сгенерированного ключа.
     Размер памяти совпадает с размером, указанном
     при создании */
  if ((shmid = shmget(key,
                      CHAR_NUMBER * sizeof(char), 0)) < 0) {
    printf("Can't find shared memory\n");
    exit(-1);
  }

  /* Пытаемся отобразить разделяемую память в адресное
     пространство текущего процесса. Обратите внимание на
     то, что для правильного сравнения мы явно
     преобразовываем значение -1 к указателю на целое. */
  if ((str = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  /* Выводим строку, храняющуюся в разделяемой памяти */
  printf("%s", str);

  /* Удаляем разделяемую память из адресного пространства
     текущего процесса и из системы и завершаем работу */
  if (shmdt(str) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }
  if (shmctl(shmid, IPC_RMID, NULL) < 0) {
    printf("Can't delete shared memory\n");
    exit(-1);
  }

  return 0;
}
