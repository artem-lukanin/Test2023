#include <stdio.h>     // printf
#include <stdlib.h>    // exit
#include <sys/types.h> // key_t
#include <sys/ipc.h>   // ftok, shm*
#include <sys/shm.h>   // shm*
#include <errno.h>     // errno
#include <sys/sem.h>   // sem*

/* Организация разделяемой памяти для массива из
   трех целых чисел. Первый элемент массива является
   счетчиком числа запусков программы 1,
   второй элемент массива – счетчиком числа
   запусков программы 2, третий элемент массива –
   счетчиком числа запусков обеих программ. */

int main() {
  int *array; // Указатель на разделяемую память

  /* IPC дескриптор для области
     разделяемой памяти */
  int shmid;

  /* Флаг необходимости инициализации
     элементов массива (создан ли уже
     сегмент разделяемой памяти) */
  int is_created = 0;

  /* Имя файла, используемое для генерации ключа.
     Файл с таким именем должен существовать
     в текущей директории */
  char pathname[] = "shmem1_sem.c";

  key_t key = 0; // IPC ключ
  /* Генерируем IPC ключ из имени файла shmem1.c в
     текущей директории и номера экземпляра области
     разделяемой памяти 0 */
  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can't generate a key\n");
    exit(-1);
  }

  /* Пытаемся эксклюзивно создать разделяемую память
     для сгенерированного ключа, т. е. если для этого
     ключа она уже существует, системный вызов вернет
     отрицательное значение. Размер памяти определяем
     как размер массива из трех целых переменных, права
     доступа 0666 – чтение и запись разрешены для всех */
  if ((shmid = shmget(key, 3 * sizeof(int),
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
         уже существует, то пытаемся получить ее IPC
         дескриптор и, в случае удачи, сбрасываем
         флаг необходимости инициализации элементов
         массива */
      if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
        printf("Can't find shared memory\n");
        exit(-1);
      }
      is_created = 1;
    }
  }

  /* Пытаемся отобразить разделяемую память в адресное
     пространство текущего процесса. Обращаем внимание на
     то, что для правильного сравнения нужно явно
     преобразовывать значение -1 к указателю на целое. */
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  // Для семафора
  int semid;
  key_t semkey;
  struct sembuf mybuf;

  // Генерация ключа для массива семафоров
  if ((semkey = ftok(pathname, 1)) < 0) {
    printf("Sem: Can't generate key\n");
    exit(-1);
  }

  /* Заполняем структуру для выполнения операций
     над семафором */
  mybuf.sem_num = 0;
  mybuf.sem_op = 1;
  mybuf.sem_flg = 0;

  // Создание семафора
  if ((semid = semget(semkey, 1,
                      0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can't create sem\n");
      exit(-1);
    } else if ((semid = semget(semkey, 1, 0)) < 0) {
      printf("Can't get semid\n");
      exit(-1);
    }
  } else {
    // Инициализация семафора единицей
    printf("First A(S, 1)\n");
    if (semop(semid, &mybuf, 1) < 0) {
      printf("Can't do the first operation A(S, 1)\n");
      exit(-1);
    }
  }

  /* В зависимости от значения флага is_created
     либо инициализируем массив,
     либо увеличиваем соответствующие счетчики */
  if (is_created) {

    // D(S, 1)
    printf("D(S, 1)\n");
    mybuf.sem_op = -1;
    if (semop(semid, &mybuf, 1) < 0) {
      printf("Can't do operation D(S, 1)\n");
      exit(-1);
    }

    array[0] += 1;

    printf("For loop\n");
    long i;
    for (i = 0; i < 5000000000L; i++);

    array[2] += 1;

    // A(S, 1)
    mybuf.sem_op = 1;
    printf("A(S, 1)\n");
    if (semop(semid, &mybuf, 1) < 0) {
      printf("Can't do the second operation A(S, 1)\n");
      exit(-1);
    }

  } else {
    array[0] = 1;
    array[1] = 0;
    array[2] = 1;
  }

  /* Печатаем новые значения счетчиков, удаляем
     разделяемую память из адресного пространства
     текущего процесса и завершаем работу */
  printf("Program 1: %d, program 2: %d, total: %d\n",
         array[0], array[1], array[2]);
  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
