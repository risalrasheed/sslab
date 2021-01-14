#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#define MAX 5

sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int buffer[MAX];
int pin=0;
int cout=0;
void *Producer(void *producer_num) {
  for(int i=0;i<MAX; i++) {
    sem_wait(&empty); //wait
    pthread_mutex_lock(&mutex);
    buffer[pin] = i;
    printf("Producer %d produced %d at %d\n",*((int *)producer_num),buffer[pin],pin);
    pin=(pin+1)%MAX;
    pthread_mutex_unlock(&mutex);
    sem_post(&full); //signal
  }
}
void *Consumer(void *consumer_num) {
  for(int i=0;i<MAX;i++) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    printf("Consumer %d consumed %d from %d\n",*((int *)consumer_num),buffer[cout],cout);
    cout=(cout+1)%MAX;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

  }
}
void main() {
  pthread_t producer[MAX],consumer[MAX];
  pthread_mutex_init(&mutex,NULL);
  sem_init(&empty,0,MAX);
  sem_init(&full,0,0);

  int a[5] = {1,2,3,4,5};

  for(int i=0;i<MAX;i++) {
    pthread_create(&producer[i],NULL, (void *) Producer, (void *)&a[i]);
  }
  for(int i=0;i<MAX;i++) {
    pthread_create(&consumer[i],NULL, (void *) Consumer, (void *)&a[i]);
  }
  for(int i=0;i<5;i++) {
    pthread_join(producer[i],NULL);
  }
  for(int i=0;i<5;i++) {
    pthread_join(consumer[i],NULL);
  }

  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);
}
