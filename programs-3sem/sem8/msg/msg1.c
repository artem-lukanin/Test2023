/* Программа получает доступ к очереди сообщений,
   отправляет в нее 5 текстовых сообщений с типом 1
   и одно пустое сообщение с типом 255,
   которое будет служить для второй программы
   сигналом прекращения работы. */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Тип сообщения для прекращения работы второй программы
#define LAST_MESSAGE 255

int main() {
  int msqid; // IPC-дескриптор для очереди сообщений
  /* Имя файла, использующееся для генерации ключа.
     Файл с таким именем должен существовать в
     текущей директории */
  char pathname[] = "msg1.c";
  key_t key; // IPC ключ

  /* Генерируем IPC-ключ
     из имени файла msq1.c в текущей директории
     и номера экземпляра очереди сообщений 0 */
  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can't generate a key\n");
    exit(-1);
  }

  /* Пытаемся получить доступ по ключу
     к очереди сообщений, если она существует,
     или создать ее
     с правами доступа read & write для всех пользователей */
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  // Пользовательская структура для сообщения
  struct mymsgbuf {
    long mtype;
    char mtext[100];
  } mybuf;

  int i = 0;
  int len = 0; // Длина информативной части сообщения

  /* Посылаем в цикле пять сообщений с типом 1
     в очередь сообщений, идентифицируемую msqid */
  for (i = 0; i < 5; i++) {
    /* Сначала заполняем структуру для нашего сообщения
       и определяем длину информативной части */
    mybuf.mtype = 1;
    strcpy(mybuf.mtext, "This is a text message");
    len = strlen(mybuf.mtext) + 1;

    /* Отсылаем сообщение. В случае ошибки сообщаем об этом
       и удаляем очередь сообщений из системы */
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can't send a message to the queue\n");
      if (msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL) < 0) {
        printf("Can't delete the queue from the system\n");
        exit(-1);
      }
      exit(-1);
    }
  }

  /* Отсылаем сообщение с типом LAST_MESSAGE и длиной 0,
     которое заставит получающий процесс прекратить работу */
  mybuf.mtype = LAST_MESSAGE;
  len = 0;
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can't send the last message to the queue\n");
    if (msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL) < 0) {
      printf("Can't delete the queue from the system\n");
      exit(-1);
    }
    exit(-1);
  }

  return 0;
}
