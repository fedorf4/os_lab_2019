#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void do_one_thing(){

    pthread_mutex_lock(&lock1);

    printf("Started do_one_thing..\n");
    sleep(1);

    printf("Trying to get another_thing\n");
    pthread_mutex_lock(&lock2); 
    printf("Aquired another_thing\n");
    pthread_mutex_unlock(&lock2);

    printf("Finished do_one_thing..\n");

    pthread_mutex_unlock(&lock1);

    pthread_exit(NULL);

}

void do_another_thing(){

    pthread_mutex_lock(&lock2);

    printf("Started do_another_thing..\n");
    sleep(1);

    printf("Trying to get another_thing\n");
    pthread_mutex_lock(&lock1); 
    printf("Aquired another_thing\n");
    pthread_mutex_unlock(&lock1);

    printf("Finished do_another_thing..\n");

    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);

}

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)do_one_thing,NULL)!= 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}