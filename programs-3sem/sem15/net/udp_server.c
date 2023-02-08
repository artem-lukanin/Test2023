/* Простой пример UDP-сервера для сервиса echo */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 51000
#define MAX_LEN 1000

int main() {
  // Структуры для адресов сервера и клиента
  struct sockaddr_in servaddr, cliaddr;

  /* Заполняем структуру для адреса сервера: семейство
     протоколов TCP/IP, сетевой интерфейс – любой, номер
     порта 51000. Поскольку в структуре содержится
     дополнительное не нужное нам поле, которое должно
     быть нулевым, перед заполнением обнуляем ее всю */
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int sockd; // Дескриптор сокета
  // Создаем UDP-сокет
  if ((sockd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(NULL); // Печатаем сообщение об ошибке
    exit(-1);
  }

  // Настраиваем адрес сокета
  if (bind(sockd, (struct sockaddr *) &servaddr,
           sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockd);
    exit(-1);
  }

  int clilen = 0;
  // Массив для принятой и отсылаемой строки
  char line[MAX_LEN];

  while (1) {
    // Основной цикл обслуживания

    /* В переменную clilen заносим максимальную
       длину для ожидаемого адреса клиента */
    clilen = sizeof(cliaddr);

    /* Ожидаем прихода запроса от клиента и читаем
       его. Максимальная допустимая длина датаграммы –
       999 символов, адрес отправителя помещаем в
       структуру cliaddr, его реальная длина будет
       занесена в переменную clilen */
    if (recvfrom(sockd, line, MAX_LEN - 1, 0,
                 (struct sockaddr *) &cliaddr, &clilen) < 0) {
      perror(NULL);
      close(sockd);
      exit(-1);
    }

    // Печатаем принятый текст на экране
    printf("%s\n", line);

    // Принятый текст отправляем обратно по адресу отправителя
    if (sendto(sockd, line, strlen(line), 0,
               (struct sockaddr *) &cliaddr, clilen) < 0) {
      perror(NULL);
      close(sockd);
      exit(-1);
    } // Уходим ожидать новую датаграмму
  }

  return 0;
}
