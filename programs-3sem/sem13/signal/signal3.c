/* Программа с пользовательской обработкой сигнала SIGINT,
   возвращающаяся к первоначальной реакции на этот сигнал
   после 5 его обработок*/

#include <signal.h>
#include <stdio.h>

// Счетчик числа обработок сигнала
int i = 0;

/* Указатель, в который будет занесен
   адрес предыдущего обработчика сигнала */
void (*p)(int);

// Функция my_handler – пользовательский обработчик сигнала
void my_handler(int nsig) {
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
    i++;
    /* После 5-й обработки возвращаем первоначальную
    реакцию на сигнал */
    if (i == 5) signal(SIGINT, p);
}

int main() {
    /* Выставляем свою реакцию процесса на сигнал SIGINT,
       запоминая адрес предыдущего обработчика */
    p = signal(SIGINT, my_handler);

    /* Начиная с этого места, процесс будет 5 раз
       печатать сообщение о возникновении сигнала SIGINT */
    while(1);
    return 0;
}
