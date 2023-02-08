/* Простой пример UDP клиента для сервиса echo */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 51000 // 7
#define MAX_LEN 1000

int main(int argc, char *argv[]) {
  /* Сначала проверяем наличие второго аргумента в
     командной строке. При его отсутствии ругаемся и
     прекращаем работу */
  if (argc < 2) {
    printf("Usage: a.out <IP address>\n");
    exit(-1);
  }

  int sockd = 0; /* Дескриптор сокета */
  // Создаем UDP сокет
  if ((sockd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(NULL); // Печатаем сообщение об ошибке
    exit(-1);
  }

  // Структуры для адресов сервера и клиента
  struct sockaddr_in servaddr, cliaddr;

  /* Заполняем структуру для адреса клиента: семейство
     протоколов TCP/IP, сетевой интерфейс – любой, номер
     порта по усмотрению операционной системы. Поскольку
     в структуре содержится дополнительное не нужное нам
     поле, которое должно быть нулевым, перед заполнением
     обнуляем ее всю */
  bzero(&cliaddr, sizeof(cliaddr));
  cliaddr.sin_family = AF_INET;
  cliaddr.sin_port = htons(0);
  cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Настраиваем адрес сокета
  if (bind(sockd, (struct sockaddr *) &cliaddr,
           sizeof(cliaddr)) < 0) {
    perror(NULL);
    // По окончании работы закрываем дескриптор сокета
    close(sockd);
    exit(-1);
  }

  /* Заполняем структуру для адреса сервера:
     семейство протоколов TCP/IP, сетевой интерфейс –
     из аргумента командной строки, номер порта 7.
     Поскольку в структуре содержится дополнительное
     не нужное нам поле, которое должно быть нулевым,
     перед заполнением обнуляем ее всю */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
    printf("Invalid IP address\n");
    // По окончании работы закрываем дескриптор сокета
    close(sockd);
    exit(-1);
  }

  // Массивы для отсылаемой и принятой строки
  char sendline[MAX_LEN], recvline[MAX_LEN];

  // Вводим строку, которую отошлем серверу
  printf("Type a string => ");
  fgets(sendline, MAX_LEN, stdin);

  // Отсылаем датаграмму
  if (sendto(sockd, sendline, strlen(sendline) + 1,
             0, (struct sockaddr *) &servaddr,
             sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockd);
    exit(-1);
  }

  /* Ожидаем ответа и читаем его. Максимальная
     допустимая длина датаграммы – 1000 символов,
     адрес отправителя нам не нужен */
  if (recvfrom(sockd, recvline, MAX_LEN, 0,
               (struct sockaddr *) NULL, NULL) < 0) {
    perror(NULL);
    close(sockd);
    exit(-1);
  }

  // Печатаем пришедший ответ и закрываем сокет
  printf("%s\n", recvline);
  close(sockd);
  return 0;
}
