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

int main() {
#define MAX 20  /* Change this constant to suit your testing. */

/*
int counter = 1;
int blocked_threads = 0;
struct sem_t {
  int counter;
  pthread_mutex_t mutex;
}
*/

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

typedef struct {
	sem_t sem;
}Pthread_cond_t;

sem_t sem;
sem_init(&sem, 0, 0);
Pthread_cond_t empty;
Pthread_cond_t fill;
pthread_mutex_t mutex;
const int loops = 100;
sem_init(&(fill.sem), 0, MAX);
sem_init(&(fill.sem), 0, 0);
pthread_mutex_init(&mutex, NULL);

int Pthread_cond_wait(Pthread_cond_t* cond, pthread_mutex_t* mutex) {
	pthread_mutex_unlock(mutex);
	int b = sem_wait(&sem);
	pthread_mutex_lock(mutex);
  return b;
}

int Pthread_cond_signal(Pthread_cond_t* cond) {
	int b = sem_post(&sem);
  return b;
}

int Pthread_cond_init(Pthread_cond_t* cond, int val) {
	int b = sem_init(&(cond->sem), 0, val);
  return b;
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

int buf[MAX];
pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
int t;
t = pthread_create(&p1, NULL, producer, NULL);
t = pthread_create(&p2, NULL, consumer, NULL);
t = pthread_create(&p3, NULL, producer, NULL);
t = pthread_create(&p4, NULL, consumer, NULL);
t = pthread_create(&p5, NULL, producer, NULL);
t = pthread_create(&p6, NULL, consumer, NULL);
t = pthread_create(&p7, NULL, producer, NULL);
t = pthread_create(&p8, NULL, consumer, NULL);

t = pthread_join(p1, NULL);
t = pthread_join(p2, NULL);
t = pthread_join(p3, NULL);
t = pthread_join(p4, NULL);
return 0;
}
