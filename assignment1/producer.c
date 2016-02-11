#include <stdio.h>
#include <pthread.h>
#define MAX 3
pthread_mutex_t the_mutex;
pthread_cond_t c1;
int n;
n = 0;

void* producer(void *ptr) {
  int i;
  for (i = 0; i < MAX; i++) {
    pthread_mutex_lock(&the_mutex);
    while (n <= 0) 
      pthread_cond_wait(&c1, &the_mutex);
    pthread_mutex_unlock(&the_mutex);

    usleep(5000);

    pthread_mutex_lock(&the_mutex); 
    printf("Consumer: %d\n", i);
    n--;
    pthread_mutex_unlock(&the_mutex); 

  }
  pthread_exit(0);
}
int main
{
  return 0;
}
