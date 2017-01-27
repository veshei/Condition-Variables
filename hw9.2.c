#include <stdio.h>
#include <pthread.h>

int main() {
#define MAX 20  /* Change this constant to suit your testing. */
int testie = 0;
pthread_mutex_t mutex;
int a = pthread_mutex_init(&mutex, NULL);

int numReaders = 0;
int numWriters = 0;
pthread_cond_t notReading;
pthread_cond_t notWriting;
int b = pthread_cond_init(&notReading, NULL);
int c = pthread_cond_init(&notWriting, NULL);

void *reader() {
// PHASE: A
// do i need to lock again?
 {
  int d = pthread_mutex_lock(&mutex);
  //numReaders++;
 }
 while (numWriters > 0) {
  pthread_cond_wait(&notWriting, &mutex);
 }
 numReaders++;
 int f = pthread_mutex_unlock(&mutex);

 // PHASE B:
 // doing the reading
 printf("%d", testie);

 //PHASE C:
 int m = pthread_mutex_lock(&mutex);
 numReaders--;
 if (numReaders = 0) {
  pthread_cond_signal(&notReading);
 }
 int n = pthread_mutex_unlock(&mutex);
}

void *writer() {
  //phase a
  int g = pthread_mutex_lock(&mutex);
  //numWriters++;
  while (numReaders > 0 || numWriters > 1) {
    if (numWriters > 1) {
      int z = pthread_cond_wait(&notWriting, &mutex);
    }
    if (numReaders > 0) {
      int y = pthread_cond_wait(&notReading, &mutex);
    }
  }
  numWriters++;
  g = pthread_mutex_unlock(&mutex);

  //Phase b
  // doing the writing
  testie += 1;

  //phase c
  int j = pthread_mutex_lock(&mutex);
  numWriters--;
  if (numWriters == 0) {
    int l = pthread_cond_signal(&notWriting);
  }
  int k = pthread_mutex_unlock(&mutex);
 }

 int t;
 pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
 t = pthread_create(&p1, NULL, writer, NULL);
 t = pthread_create(&p2, NULL, reader, NULL);
 t = pthread_create(&p3, NULL, writer, NULL);
 t = pthread_create(&p4, NULL, reader, NULL);
 t = pthread_create(&p5, NULL, writer, NULL);
 t = pthread_create(&p6, NULL, reader, NULL);
 t = pthread_create(&p7, NULL, writer, NULL);
 t = pthread_create(&p8, NULL, reader, NULL);

 t = pthread_join(p1, NULL);
 t = pthread_join(p2, NULL);
 t = pthread_join(p3, NULL);
 t = pthread_join(p4, NULL);
 t = pthread_join(p5, NULL);
 t = pthread_join(p6, NULL);
 t = pthread_join(p7, NULL);
 t = pthread_join(p8, NULL);
 return 0;
}