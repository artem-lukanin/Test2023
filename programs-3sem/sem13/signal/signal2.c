/* Программа с пользовательской обработкой
   сигналов SIGINT и SIGQUIT*/

#include <signal.h>
#include <stdio.h>

// Функция my_handler – пользовательский обработчик сигналов
void my_handler(int nsig) {
  if (nsig == SIGINT)
    printf("Receive signal %d, CTRL-C was pressed\n", nsig);
  else if (nsig == SIGQUIT)
    printf("Receive signal %d, CTRL-\\ was pressed\n", nsig);
}

int main(void){
  // Выставляем реакцию процесса на сигналы SIGINT и SIGQUIT
  signal(SIGINT, my_handler);
  signal(SIGQUIT, my_handler);

  /* Начиная с этого места, процесс будет печатать
     сообщение о возникновении сигналов SIGINT и SIGQUIT */
  while(1);
  return 0;
}
