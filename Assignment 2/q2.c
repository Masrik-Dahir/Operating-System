/*
 * Masrik Dahir
 * Date: 03/28/2022
 * The producer thread randomly select a integer (out of 0 to 100) and add it to the bounded buffer. The consumer thread dequeue the 
 * integer. The FIFO statigy is used in the bounded buffer to queue elements in first while dequeuing them first
 * The q2.c impolements the correct version of semaphore
 * 
 * 
 * Usage Guide
 * (1) gcc -g q2.c -o q2.exe -lpthread
 * (2) ./q2.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>

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

pthread_mutex_t buffer_mutex;

struct binary_semaphore full_sem;
struct binary_semaphore empty_sem;


typedef struct node {
	int value;
	struct node *previous;
	struct node *next;
}Buffer;

Buffer *head = NULL, *tail = NULL;
int first_element;

int length()
{
	int size = 0;
	Buffer *current = head;
	while(current != NULL) {
		size++;
		current = current->next;
	}
	return size;
}

void Print_Buffer(){
	Buffer *current = head;
	if (head == NULL) {
		assert(tail == NULL);
		
		return;
	}
    printf("Current buffer = [");
	while(current != NULL) {
        printf("%d", current->value);
        current = current->next;
        if (current != NULL){
            printf(", ");
        }
	}
	printf("]\n\n");
	return;
}

void Insert_First(int value)
{
	Buffer *current; 

	if (current = (Buffer *) malloc(sizeof (Buffer))) {
		current->value = value;
		current->next = NULL;
		current->previous = NULL;
	} else {
		printf("unable to allocate memory \n");
		return;
	}
		
	if (tail == NULL) {
		assert (tail == NULL);
		head = tail = current;
	} else {
		current->previous = tail;
		tail->next = current;
		tail = current;
	}
	
	return;
}

void Delete_First()
{
	Buffer *temp = head;

	if (head == NULL) {
		assert(tail == NULL);
		
		return;
	}

	if (head == tail) {
		head = tail = NULL;
	} else {
		head = head->next;
		head->previous = NULL;
	}
	
    first_element = temp->value;
	free(temp);
	
	return ;
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
 
buffer_t dequeuebuffer(int thread_numb) {
    if (length() > 0) {
        Delete_First();
        printf("Consumer %d dequeue %d from buffer\n", thread_numb, first_element);
        Print_Buffer();
        return first_element;
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
        value = dequeuebuffer(thread_numb);
        pthread_mutex_unlock(&buffer_mutex);
        Semaphore_Post(&full_sem);
        
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