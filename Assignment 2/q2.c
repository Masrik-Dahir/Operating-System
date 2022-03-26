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

struct buffer {
   int value;
   int key;
   struct buffer *next;
};

struct buffer *head = NULL;
struct buffer *current = NULL;

typedef int buffer_t;

pthread_mutex_t buffer_mutex;

struct binary_semaphore full_sem;
struct binary_semaphore empty_sem;

//display the list
void Print_Buffer() {
   struct buffer *ptr = head;
	printf("Current buffer = [");
   //start from the beginning
   while(ptr != NULL) {
      printf("%d", ptr->value);
      ptr = ptr->next;
      if (ptr != NULL){
          printf(", ");
      }
   }
   printf("]\n\n");
	
}

//insert link at the first location
void Insert_First(int value) {
   //create a link
   struct buffer *link = (struct buffer*) malloc(sizeof(struct buffer));
	
   link->value = value;
	
   //point it to old first buffer
   link->next = head;
	
   //point first to new first buffer
   head = link;
}

//delete first item
struct buffer* Deleter_First() {

   //save reference to first link
   struct buffer *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}

int length() {
   int length = 0;
   struct buffer *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

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

void insertbuffer(int value, int thread_numb) {
    if (length() < SIZE) {
        Insert_First(value);
        printf("Producer %d added %d to buffer\n", thread_numb, value);
        Print_Buffer();
            
    } else {
        printf("Buffer overflow\n");
    }
}
 
buffer_t dequeuebuffer() {
    if (length() > 0) {
        struct buffer *temp = Deleter_First();
        return temp->value;
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
        // sleep(rand() % 10);
        value = rand() % 100;
        Semaphore_Wait(&full_sem); 
        pthread_mutex_lock(&buffer_mutex); 
        insertbuffer(value, thread_numb);
        pthread_mutex_unlock(&buffer_mutex);
        Semaphore_Post(&empty_sem); 
        
    }
    pthread_exit(0);
}
 
void *consumer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    int value;
    int i=0;
    while (i++ < PRODUCER_LOOPS) {
        sleep(rand() % 10);
        Semaphore_Wait(&empty_sem);
        pthread_mutex_lock(&buffer_mutex);
        value = dequeuebuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        Semaphore_Post(&full_sem);
        printf("Consumer %d dequeue %d from buffer\n", thread_numb, value);
        Print_Buffer();
   }
    pthread_exit(0);
}
 
int main(int argc, int **argv) {
    clock_t start_time = clock();

    srand(time(NULL));

    pthread_mutex_init(&buffer_mutex, NULL);

    Semaphore_Init(&full_sem, SIZE);
    Semaphore_Init(&empty_sem, 0);

    int process_pid[NUMB_PRODUCERS];
    pthread_t thread[NUMB_PRODUCERS + NUMB_CONSUMERS];
    int thread_numb[NUMB_PRODUCERS + NUMB_CONSUMERS];

    int thread_id = 0;
    for (int i = 0; i < NUMB_PRODUCERS; i++) {
        thread_numb[thread_id] = i;
        pthread_create(thread + thread_id, 
                       NULL, 
                       producer, 
                       thread_numb + thread_id);  
        thread_id ++;
    }
    
    for (int i = 0; i < NUMB_CONSUMERS; i++) {
        thread_numb[thread_id] = i;
        pthread_create(thread + thread_id, 
                       NULL, 
                       consumer, 
                       thread_numb + thread_id);
        thread_id ++;
    }
    
    for (int i = 0; i < NUMB_PRODUCERS + NUMB_CONSUMERS; i++){
        pthread_join(thread[i], NULL);
    }
    
    printf("Elapsed Time: %f seconds\n", ((double)clock() - start_time)/CLOCKS_PER_SEC);

    pthread_mutex_destroy(&buffer_mutex);

    Semaphore_Destroy(&full_sem);
    Semaphore_Destroy(&empty_sem);
 
    return 0;
}