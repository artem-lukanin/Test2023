/* Программа получает доступ к одному системному
   семафору, ждет, пока его значение не станет больше
   или равным 1 после запусков программы sem2.c,
   а затем уменьшает его на 1 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main() {
  int semid; // IPC-дескриптор для массива IPC семафоров
  /* Имя файла, использующееся для генерации ключа.
     Файл с таким именем должен существовать
     в текущей директории */
  char pathname[] = "sem1.c";
  key_t key; // IPC ключ
  // Структура для задания операции над семафором
  struct sembuf mybuf;

  /* Генерируем IPC-ключ из имени файла sem1.c в
     текущей директории и номера экземпляра
     массива семафоров 0 */
  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can't generate a key\n");
    exit(-1);
  }

  /* Пытаемся получить доступ по ключу к массиву
     семафоров, если он существует,
     или создать его из одного семафора,
     если его еще не существует, с правами
     доступа read & write для всех пользователей */
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can't get semid\n");
    exit(-1);
  }

  /* Выполним операцию D(semid, 1) для нашего массива
     семафоров. Для этого сначала заполним нашу структуру.
     Флаг, как обычно, полагаем равным 0. Наш массив
     семафоров состоит из одного семафора с номером 0.
     Код операции -1. */
  mybuf.sem_op = -1;
  mybuf.sem_flg = 0;
  mybuf.sem_num = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can't wait for condition\n");
    exit(-1);
  }

  /*
  // Удаляем массив семафоров из системы
  if (semctl(semid, 0, IPC_RMID, 0) < 0) {
    printf("Can't delete the semaphore\n");
    exit(-1);
  }
  */

  printf("End of program 1\n");

  return 0;
}
