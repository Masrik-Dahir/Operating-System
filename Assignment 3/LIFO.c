/*
 * Masrik Dahir
 * Date: 03/28/2022
 * 
 * 
 * Usage Guide
 * (1) gcc -g LIFO.c -o LIFO.exe -lpthread
 * (2) ./LIFO.exe
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>

// static bufferArray size
#define BUFFER_SIZE 30

//semaphore struct
typedef struct c_sem {
    int val;
    sem_t gate;
    sem_t mutex;
} c_sem;

// sem_init function
void Semaphore_Init(c_sem *cs, int K) {
    cs->val = K;
    if(K > 0)
        sem_init(&cs->gate, 1, 1);
    else
        sem_init(&cs->gate, 1, 0);
    sem_init(&cs->mutex, 1, 1);
}

// sem_destroy function
void Semaphore_Destroy(c_sem *cs) {
    sem_destroy(&cs->gate);
    sem_destroy(&cs->mutex);
}

// sem_wait function
void Semaphore_Wait(c_sem *cs) {
    sem_wait(&cs->gate);
    sem_wait(&cs->mutex);
    cs->val = cs->val - 1;
    if (cs->val > 0)
        sem_post(&cs->gate);
    sem_post(&cs->mutex);
}

// sem_post function
void Semaphore_Post(c_sem *cs) {
    sem_wait(&cs->mutex);
    cs->val = cs->val + 1;
    if(cs->val == 1)
        sem_post(&cs->gate);
    sem_post(&cs->mutex);
}

//time_in_between returns the difference in time from start to end pointer
float time_in_between(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
}

//print_job struct
typedef struct print_job {
    pid_t pid;
    int size;
    struct timespec start_time;
} print_job;

//the shared memory struct has the vaqriables in used by used by multiple processes
typedef struct shm_struct {
    c_sem fullSemaphore;
    c_sem emptySemaphore;
    c_sem binarySemaphore;
    c_sem processLock;

    //producer variables
    int numOfProducers;
    int producersEnd;

    //parentPid process variables
    int printJobMade;
    int printJobProcessed;
    int bytesProduced;
    int bytesConsumed;

    //bufferArray array
    print_job bufferArray[BUFFER_SIZE];

    //top element in the stack bufferArray
    int index;

} shm_struct;

//global variables
pid_t parentPid;
int numOfThreads;
int numOfProcesses;
shm_struct* shm;        
int shmid;              
shm_struct* ref;        
pthread_t *threads;     
double timeEclipsed;

// insertign elements in the bufferArray
void insertbuffer(print_job value, shm_struct* shm) {
    if(shm->index < BUFFER_SIZE)
        shm->bufferArray[shm->index++] = value;
    else
        printf("Buffer overflow\n");
}

// dequeueing elements from the bufferArray
print_job dequeuebuffer(shm_struct* shm) {
    if(shm->index > 0)
        return shm->bufferArray[--shm->index];
    else {
        printf("Buffer underflow\n");
        exit(1);
    }
}

// producer processes
void producer() {
    //unique RNG seed for processes
    srand(getpid());

    //jobs
    int num_of_jobs = rand() % 30 + 1;
    ref->printJobMade += num_of_jobs;

    //process variables
    print_job value;
    value.pid = getpid();
    int i, bytesSize;

    // Iterating the jobs
    for(i = 0; i < num_of_jobs; i++) {

        //determine print_job.size
        bytesSize = rand() % 901 + 100;
        value.size = bytesSize;

        //critical section
        Semaphore_Wait(&ref->emptySemaphore);
        Semaphore_Wait(&ref->binarySemaphore);
        clock_gettime(CLOCK_REALTIME, &value.start_time);
        insertbuffer(value, ref);
        ref->bytesProduced += bytesSize;

        // printing the queuing status 
        printf("Producer %u added %d to bufferArray\n", getpid(), bytesSize);

        Semaphore_Post(&ref->binarySemaphore);
        Semaphore_Post(&ref->fullSemaphore);

        //sleep
        usleep(rand() % (1000000 - 100000 + 1) + 100000);
    }

    //finishing processes by signalling
    Semaphore_Wait(&ref->processLock);
    ref->producersEnd++;
    Semaphore_Post(&ref->processLock);

    // exting
    exit(0);
}

// consumer threads 
void *consumer(void *thread_n) {
    int thread = *(int *)thread_n;
    print_job val;
    struct timespec timeFinished;

    //interating to validate correct total number of jobs
    while(ref->producersEnd < ref->numOfProducers || ref->printJobProcessed < ref->printJobMade) {
        
        //critical section
        Semaphore_Wait(&ref->fullSemaphore);
        Semaphore_Wait(&ref->binarySemaphore);
        clock_gettime(CLOCK_REALTIME, &timeFinished);
        val = dequeuebuffer(ref);
        ref->printJobProcessed++;
        ref->bytesConsumed += val.size;

        // Printitng Dequeuing status
        printf("Consumer %d dequeue (%u, %d) from bufferArray\n", thread, val.pid, val.size);

        Semaphore_Post(&ref->binarySemaphore);
        Semaphore_Post(&ref->emptySemaphore);

        //calcualting total time
        timeEclipsed += time_in_between(&val.start_time, &timeFinished);
        
        //cancel all other threads when teh final job is done
        if(ref->printJobMade == ref->printJobProcessed) {
            int i;
            for(i = 0; i < numOfThreads; i++) {
                if(threads[i] != pthread_self())
                    pthread_cancel(threads[i]);
            }
            pthread_exit(0);
        }

        // To determine First in first out, adding usleep in the consumer thread, so there is a delay in dequeuing
        // usleep(1000000);
    }

}

//signal handler
void signalHandler(int signum) {

    //the parent process has a special cancellation routine
    if(getpid() != parentPid) {
        
        //detaching shared memory
        if ( shmdt(shm) == -1)
            fprintf(stderr, "Unable to detach\n");
    }
    else {
        //destroying the semaphores
        Semaphore_Destroy(&ref->emptySemaphore);
        Semaphore_Destroy(&ref->fullSemaphore);
        Semaphore_Destroy(&ref->binarySemaphore);
        Semaphore_Destroy(&ref->processLock);
        
        //cancelling all threads
        int i, ret;
        for(i = 0; i < sizeof(threads) / sizeof(pthread_t); i++)
            ret = pthread_cancel(threads[i]);

        //removing the shared memory
        if ( shmdt(shm) == -1)
            fprintf(stderr, "Unable to detach\n");
        shmctl(shmid, IPC_RMID, NULL);
    }
    exit(0);
}

int main(int argc, char *argv[]) {

    //signal handler
    signal(SIGINT, signalHandler);

    //parent pid
    parentPid = getpid();

    //Getting number of processes and threads from the commandline argument
    if(argc == 3) {
        numOfProcesses = atoi(argv[1]);
        numOfThreads = atoi(argv[2]);
    }
    else {

        // Returning error because there is either no number of processes, no number of threads, or both
        perror("You must enter in the number of processes and the number of threads\n");
        exit(1);
    }

    //random key
    int randomKey = rand() % 9999;
    
    //shared memory
    if((shmid = shmget(randomKey, sizeof(shm_struct), IPC_CREAT | 0600)) < 0 )
    {
        perror("Shared Memory Segment Get Failed");
        exit(1);
    }
    if( (shm = shmat(shmid, NULL, 0)) == (shm_struct*) -1 ) {
        perror("Shared Memory Attach Failed");
        exit(1);
    }

    //shared memory variables
    ref = shm;
    ref->numOfProducers = numOfProcesses;
    ref->index = 0;
    ref->producersEnd = 0;
    ref->printJobMade = 0;
    ref->printJobProcessed = 0;
    ref->bytesProduced = 0;
    ref->bytesConsumed = 0;

    //initiating the semaphores
    Semaphore_Init(&ref->emptySemaphore, BUFFER_SIZE);
    Semaphore_Init(&ref->fullSemaphore, 0);
    Semaphore_Init(&ref->binarySemaphore, 1);
    Semaphore_Init(&ref->processLock, 1);

    timeEclipsed = 0;

    //starting the timer
    struct timespec t_s;
    struct timespec t_e;
    clock_gettime(CLOCK_REALTIME, &t_s);

    //create the processes
    int processes;
    for(processes = 0; processes < numOfProcesses; processes++)
        if(parentPid == getpid())
            fork();

    //checking for the appropriate branch
    if(parentPid == getpid()) {

        //creating the threads
        pthread_t consumer_threads[numOfThreads];
        int thread;
        for(thread = 0; thread < numOfThreads; thread++) {
            pthread_create(consumer_threads + thread, NULL, consumer, &thread);
        }
        threads = consumer_threads;
        for(thread = 0; thread < numOfThreads; thread++)
            pthread_join(consumer_threads[thread], NULL);
    }
    else
        //executing producer
        producer();

    //finishing timing
    clock_gettime(CLOCK_REALTIME, &t_e);

    //Summary and time
    printf("Producers submitted %d jobs totaling %d bytes.\n\nConsumers processed %d jobs totaling %d bytes.\n", ref->printJobMade, ref->bytesProduced, ref->printJobProcessed, ref->bytesConsumed);
    printf("Total time taken: %f\n", time_in_between(&t_s, &t_e));
    printf("Average job time in queue: %f\n", timeEclipsed / ref->printJobProcessed);

    //destroying semaphores
    Semaphore_Destroy(&ref->emptySemaphore);
    Semaphore_Destroy(&ref->fullSemaphore);
    Semaphore_Destroy(&ref->binarySemaphore);
    Semaphore_Destroy(&ref->processLock);
    
    //detaching and removing shared memory
    if ( shmdt(shm) == -1) {
        fprintf(stderr, "Unable to detach\n");
    }

    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}