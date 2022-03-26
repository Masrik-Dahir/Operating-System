#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
 
#define SIZE 5
#define NUMB_PRODUCERS 3
#define NUMB_CONSUMERS 3
#define PRODUCER_LOOPS 2
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))

typedef struct binary_semaphore binary_semaphore;

struct binary_semaphore{
    int value;
    sem_t gate;
    sem_t mutex;
};

typedef int buffer_t;
int buffer[SIZE];
int buffer_index;
int queue_index;
int buffer_size;

pthread_mutex_t buffer_mutex;
struct binary_semaphore full_sem;
struct binary_semaphore empty_sem;

// Reader Writer variables
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

void insertbuffer(int value) {
    if (buffer_index < SIZE) {
        buffer[buffer_index++] = value;
    } else {
        printf("Buffer overflow\n");
    }
}
 
buffer_t dequeuebuffer() {
    if (buffer_index > 0) {
        return buffer[--buffer_index]; 
    } else {
        printf("Buffer underflow\n");
    }
    return 0;
}
 
 
void *producer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    int value;
    int i=0;
    while (i++ < PRODUCER_LOOPS) {
        sleep(rand() % 10);
        value = rand() % 100;

        Semaphore_Wait(&full_sem); 
        pthread_mutex_lock(&buffer_mutex); 
        insertbuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        Semaphore_Post(&empty_sem); 
        printf("Producer %d added %d to buffer\n", thread_numb, value);
    }
    pthread_exit(0);
}
 
void *consumer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    int value;
    int i=0;
    while (i++ < PRODUCER_LOOPS) {
        Semaphore_Wait(&empty_sem);
        pthread_mutex_lock(&buffer_mutex);
        value = dequeuebuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        Semaphore_Post(&full_sem);
        printf("Consumer %d dequeue %d from buffer\n", thread_numb, value);
   }
    pthread_exit(0);
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
    sem_init(&wrt, 0, 1);
    sem_init(&bsem, 0, 5);

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
    sem_destroy(&wrt);
    sem_destroy(&bsem);

    return 0;
    
}