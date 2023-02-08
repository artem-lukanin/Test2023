#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <sys/types.h> // key_t
#include <sys/ipc.h>   // ftok, shm*
#include <sys/shm.h>   // shm*
#include <errno.h>     // errno
#include <fcntl.h>     // open
#include <unistd.h>    // read

#define CHAR_NUMBER 10000

int main() {
  char *str; // Указатель на разделяемую память

  int shmid; /* IPC дескриптор для области
                разделяемой памяти */
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

  /* Пытаемся создать разделяемую память
     для сгенерированного ключа, т. е. если для этого
     ключа она уже существует, системный вызов вернет
     отрицательное значение.
     Размер памяти определяем как размер массива из
     CHAR_NUMBER переменных, права доступа 0666 –
     чтение и запись разрешены для всех */
  if ((shmid = shmget(key, CHAR_NUMBER * sizeof(char),
                      0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    /* В случае ошибки пытаемся определить: возникла ли
    она из-за того, что сегмент разделяемой памяти уже
    существует или по другой причине */
    if (errno != EEXIST) {
      // Если по другой причине – прекращаем работу
      printf("Can't create shared memory\n");
      exit(-1);
    } else {
      /* Если из-за того, что разделяемая память
         уже существует, то пытаемся получить ее
         IPC дескриптор */
      if ((shmid = shmget(key,
                          CHAR_NUMBER * sizeof(char), 0)) < 0) {
        printf("Can't find shared memory\n");
        exit(-1);
      }
    }
  }

  /* Пытаемся отобразить разделяемую память в адресное
     пространство текущего процесса. Обратите внимание на
     то, что для правильного сравнения мы явно
     преобразовываем значение -1 к указателю на целое. */
  if ((str = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  /* Получаем файловый дескриптор для данной программы */
  int fd = -1;
  if ((fd = open(pathname, O_RDONLY)) < 0) {
    printf("Can't open file\n");
    exit(-1);
  }

  /* Записываем код программы в разделяемую память */
  if (read(fd, str, CHAR_NUMBER) < 0) {
    printf("Can't read the file\n");
    exit(-1);
  }

  /* Удаляем разделяемую память из адресного пространства
     текущего процесса и завершаем работу */
  if (shmdt(str) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
