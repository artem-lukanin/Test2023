/* Простой пример TCP-клиента для сервиса echo */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 51000 // 7
#define MAX_LEN 1000

void main(int argc, char *argv[]) {
  /* Сначала проверяем наличие второго аргумента в командной строке.
     При его отсутствии прекращаем работу */
  if (argc < 2){
    printf("Usage: a.out <IP address>\n");
    exit(-1);
  }

  // Массивы для отсылаемой и принятой строки
  char sendline[MAX_LEN],recvline[MAX_LEN];
  // Обнуляем символьные массивы
  bzero(sendline, MAX_LEN);
  bzero(recvline, MAX_LEN);

  // Дескриптор сокета
  int sockd = 0;
  // Создаем TCP-сокет
  if ((sockd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    // Печатаем сообщение об ошибке
    perror(NULL);
    exit(-1);
  }

  // Структура для адреса сервера
  struct sockaddr_in servaddr;

  /* Заполняем структуру для адреса сервера:
     семейство протоколов TCP/IP,
     сетевой интерфейс – из аргумента командной строки,
     номер порта 7.
     Поскольку в структуре содержится дополнительное не нужное нам поле,
     которое должно быть нулевым, перед заполнением обнуляем ее всю */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
    printf("Invalid IP address\n");
    close(sockd);
    exit(-1);
  }

  /* Устанавливаем логическое соединение через
     созданный сокет с сокетом сервера,
     адрес которого мы занесли в структуру */
  if (connect(sockd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockd);
    exit(-1);
  }

  /* Три раза в цикле вводим строку с клавиатуры,
     отправляем ее серверу и читаем полученный ответ */
  int i = 0;
  for (i = 0; i < 3; i++) {
    printf("String => ");
    fflush(stdin);
    fgets(sendline, MAX_LEN, stdin);

    if (write(sockd, sendline, strlen(sendline) + 1) < 0) {
      perror("Can't write\n");
      close(sockd);
      exit(-1);
    }

    if (read(sockd,recvline, MAX_LEN - 1) < 0) {
      perror("Can't read\n");
      close(sockd);
      exit(-1);
    }
    printf("%s", recvline);
  }

  // Завершаем соединение
  close(sockd);
}
