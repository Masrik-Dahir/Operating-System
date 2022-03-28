/*
 * Masrik Dahir
 * Date: 03/28/2022
 * The reader thread reads cnt value. The pupose of the asssignment is to Upper bound numnber of reader
 * accessing the critical section. In my case I uppper bounded my reader to 5. Therefore at any time only
 * 5 reader reader can read; other readers has to wait for previous reader to complete before reading. 
 * The writer thread modified the cnt.
 * 
 * 
 * Usage Guide
 * (1) gcc -g q3.c -o q3.exe -lpthread
 * (2) ./q3.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))
#define UPPER_BOUND 5

typedef struct binary_semaphore binary_semaphore;

struct binary_semaphore{
    int value;
    sem_t gate;
    sem_t mutex;
};

pthread_mutex_t buffer_mutex;
struct binary_semaphore full_sem;
struct binary_semaphore empty_sem;

sem_t wrt, bsem;
pthread_mutex_t mutex;
int cnt = 1;
int numreader = 0;

void Semaphore_Init(binary_semaphore* sem, int K){
    sem->value = K;
    sem_init(&sem->gate, 0, min(1,K));
    sem_init(&sem->mutex,0,1);
}

void Semaphore_Destroy(binary_semaphore* sem){
    sem->value = 0;
    sem_destroy(&sem->gate);
    sem_destroy(&sem->mutex);
}

void Semaphore_Wait(binary_semaphore* sem){
    sem_wait(&sem->gate);
    sem_wait(&sem->mutex);

    sem->value--;

    if(sem->value > 0){
        sem_post(&sem->gate);
    }
    sem_post(&sem->mutex);
}

void Semaphore_Post(binary_semaphore* sem){
    sem_wait(&sem->mutex);
    sem->value++;
    if(sem->value == 1){
        sem_post(&sem->gate);
    }
    sem_post(&sem->mutex);
}
 
void *writer(void *wno)
{   
    Semaphore_Wait(&wrt);
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
    Semaphore_Post(&wrt);

}

void *reader(void *rno)
{   
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        Semaphore_Wait(&wrt); // If this id the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);


    // Reading Section
    Semaphore_Wait(&bsem);
    printf("Reader %d Started: read cnt as %d\n",*((int *)rno),cnt);
    sleep(2);
    printf("Reader %d Completed: read cnt as %d\n",*((int *)rno),cnt);
    Semaphore_Post(&bsem);

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        Semaphore_Post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   

    pthread_t read[10],write[5];
    pthread_mutex_init(&mutex, NULL);
    Semaphore_Init(&wrt, 1);
    Semaphore_Init(&bsem, UPPER_BOUND);

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    Semaphore_Destroy(&wrt);
    Semaphore_Destroy(&bsem);

    return 0;
    
}