// NOTE:  You must still write a main routine for this.  The constants
//        MAX and loops were chosen arbitrarily, and can be changed.
//        Your code must work for large values for these constants.
// NOTE:  Don't forget that you will compile this using:
//          gcc THIS_FILE -lpthread  # -lpthread means
//           "libpthread.a" (if -static is used), or "libpthread.so" (default)
//          OR THE SHORTER VERSION:  gcc THIS_FILE -pthread

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX 20  /* Change this constant to suit your testing. */

int buffer[MAX];
int fill_ptr = 0;
int use_ptr  = 0;
int count    = 0;

void put(int value) {
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get() {
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

typedef struct Pthread_cond_t{
  sem_t sem;
  pthread_mutex_t mutex;
}Pthread_cond_t;

Pthread_cond_t empty;
Pthread_cond_t fill;
pthread_mutex_t mutex;
const int loops = 100;

int Pthread_cond_wait(Pthread_cond_t *cond, pthread_mutex_t *mutex) {
  pthread_mutex_unlock(mutex);
  int temp = sem_wait(&(cond->sem));
  pthread_mutex_lock(mutex);
  return temp;
}

int Pthread_cond_signal(Pthread_cond_t *cond) {
  int temp = sem_post(&(cond->sem));
  return temp;
}

int Pthread_cond_init(Pthread_cond_t *cond, const pthread_condattr_t *attr) {
  int temp = sem_init(&(cond->sem), 0, 0);
  pthread_mutex_init(&mutex, NULL);
  return temp;
}

void *producer(void * arg) {
  int i;
  for (i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);            // p1
    while (count == MAX)                   // p2
      Pthread_cond_wait(&empty, &mutex);   // p3
    put(i);                                // p4
    Pthread_cond_signal(&fill);            // p5
    pthread_mutex_unlock(&mutex);          // p6
  }
}

void * consumer(void * arg) {
  int i;
  for (i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);            // c1
    while (count == 0)                     // c2
      Pthread_cond_wait(&fill, &mutex);    // c3
    int tmp = get();                       // c4
    Pthread_cond_signal(&empty);           // c5
    pthread_mutex_unlock(&mutex);          // c6
    printf("%d\n", tmp);
  }
}

int test = 0;
//pthread_mutex_t mutex;
//int a = Pthread_mutex_init(mutex, NULL);

int numReaders = 0;
int numWriters = 0;
Pthread_cond_t notReading;
Pthread_cond_t notWriting;

void *reader() {
// PHASE: A
// do i need to lock again?
for (int i = 0; i < loops; i++) {
  pthread_mutex_lock(&mutex);
  //numReaders++;
 while (numWriters > 0) {
  Pthread_cond_wait(&notWriting, &mutex);
 }
 numReaders++;
 pthread_mutex_unlock(&mutex);

 // PHASE B:
 // doing the reading
 //printf("%d", test);
 printf("Reader\n");

 //PHASE C:
 pthread_mutex_lock(&mutex);
 numReaders--;
 //if (numReaders == 0) {
  Pthread_cond_signal(&notReading);
 //}
 pthread_mutex_unlock(&mutex);
}
}

void *writer() {
  //phase a
  for (int i = 0; i < loops; i++) {
  pthread_mutex_lock(&mutex);
  //numWriters++;
  while (numReaders > 0 || numWriters > 0) {
  	if (numReaders > 0) {
      Pthread_cond_wait(&notReading, &mutex);
  	}
  	if (numWriters > 0) {
  		Pthread_cond_wait(&notWriting, &mutex);
  	}
  } 
  numWriters++;
  pthread_mutex_unlock(&mutex);

  //Phase b
  // doing the writing
  //test += 1;
  printf("Writer\n");

  //phase c
  pthread_mutex_lock(&mutex);
  numWriters--;
  //if (numWriters == 0) {
    Pthread_cond_signal(&notWriting);
  //}
  pthread_mutex_unlock(&mutex); 
  }
 }

 int main() {
 	Pthread_cond_init(&empty, NULL);
 	Pthread_cond_init(&fill, NULL);
 	Pthread_cond_init(&notReading, NULL);
    Pthread_cond_init(&notWriting, NULL);
 int p;
 pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
 p = pthread_create(&p1, NULL, writer, NULL);
 p = pthread_create(&p2, NULL, reader, NULL);
 p = pthread_create(&p3, NULL, writer, NULL);
 p = pthread_create(&p4, NULL, reader, NULL);
 p = pthread_create(&p5, NULL, writer, NULL);
 p = pthread_create(&p6, NULL, reader, NULL);
 p = pthread_create(&p7, NULL, writer, NULL);
 p = pthread_create(&p8, NULL, reader, NULL);

 p = pthread_join(p1, NULL);
 p = pthread_join(p2, NULL);
 p = pthread_join(p3, NULL);
 p = pthread_join(p4, NULL);
 p = pthread_join(p5, NULL);
 p = pthread_join(p6, NULL);
 p = pthread_join(p7, NULL);
 p = pthread_join(p8, NULL);
 return 0;
}

