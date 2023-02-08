#include <pthread.h> // pthread*
#include <stdio.h>
#include <stdlib.h>  // exit

/* Переменная a является глобальной для всей программы,
   поэтому она будет разделяться обеими нитями исполнения. */
int a = 0;

/* Функция, которая будет
   ассоциирована со вторым thread'ом */
void *create_routine(void *arg) {
  /* Параметр arg в этой функции не используется и
     присутствует только для совместимости типов данных.
     По той же причине функция возвращает значение
     void *, хотя это никак не используется в программе. */

  // Запрашиваем идентификатор нити исполнения (thread'а)
  pthread_t my_thid = pthread_self();
  a = a + 1;
  printf("Thread %ld, a = %d\n", my_thid, a);
  return NULL;
}

// Функция main() – ассоциированная функция главного thread'а
int main() {
  pthread_t thid, my_thid;
  int res = 0;

  /* Пытаемся создать новую нить исполнения,
  ассоциированную с функцией create_routine(). Передаем ей
  в качестве параметра значение NULL. В случае удачи в
  переменную thid занесется идентификатор нового
  thread'а. Если возникнет ошибка, то прекратим
  работу. */
  res = pthread_create(&thid,
                       (pthread_attr_t *)NULL,
                       create_routine, NULL);
  if (res != 0) {
    printf("Thread creation is failed with code %d\n", res);
    exit(-1);
  }

  printf("Thread was created, thid = %ld\n", thid);

  /* Запрашиваем идентификатор главного thread'а */
  my_thid = pthread_self();

  a = a + 1;
  printf("Main thread %ld, a = %d\n", my_thid, a);

  /* Ожидаем завершения порожденного thread'a,
     не интересуясь, какое значение он нам вернет.
     Если не выполнить вызов этой функции,
     то возможна ситуация, когда мы завершим функцию main()
     до того, как выполнится порожденный thread,
     что автоматически повлечет за собой его завершение,
     исказив результаты. */
  pthread_join(thid, (void **)NULL);

  return 0;
}
