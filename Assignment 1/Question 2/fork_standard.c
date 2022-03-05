/*
 * Masrik Dahir
 * Date: 02/20/2022
 * Standard fork firsts forks twice and save it to n1 and n2, where the first process is process_a (n1=1,n2=1)
 * Based on the n1 and n2 value it creates two childern of process in the same level (n1=0,n2=1 and n1=1,n2=0)
 * So, it is a nested fork() running three processes
 * 
 * 
 * Usage Guide
 * (1) gcc fork_standard.c -o fs
 * (2) ./fs
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <stdbool.h>
#include <unistd.h>
#define SHMSZ     27

int main()
{

    int n1 = fork();
    int n2 = fork();
    int val;

    if (n1 > 0 && n2 > 0){
        // printf("Process 1\n");  
        process_a();
    }

    if (n1 > 0 && n2 == 0){
        // printf("Process 2\n");
        process_b();
        
    }

    if (n1 == 0 && n2 > 0){
        // printf("Process 3\n");
        process_c();
        
    }

    if (n1 == 0 && n2 == 0){
        // printf("Process 4\n");
        
    }
       
}

int process_a()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    char new[] = "I am Process A";
    char last[] = "Goodbye";

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 1;


    int nhmid;
    key_t int_key;
    char *nhm;
    char *n;
    int_key = 2;



    /*
     * Create the segment.
     */
    if( (shmid = shmget(key, SHMSZ, IPC_CREAT | 0644)) < 0 )
    {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if( (shm = shmat(shmid, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }
    /*
     * Now put some things into the memory for the
     * other process to read.
     */


    if((nhmid = shmget(int_key, SHMSZ, IPC_CREAT | 0644)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (nhm = shmat(nhmid, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }


    s = shm;
    n = nhm;

    for( c = 0; c < sizeof new; c++ )
        *s++ = new[c]; /* post fix */
    *s = (char) NULL;
    *n = '1';
    /*
     * Finally, we wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */

    for (s = shm; *s != (char) NULL; s++)
        putchar(*s);
    putchar('\n');


    bool now = true;
    while( !(now == false && *nhm == '3'))
//        sleep(1);
        if (*nhm == '2' && now == true){
            for (s = shm; *s != (char) NULL; s++)
                putchar(*s);
            putchar('\n');
            now = false;
        }

        if (*nhm == '3' && now == false){
            for (s = shm; *s != (char) NULL; s++)
                putchar(*s);
            putchar('\n');
            now = true;
        }


    if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    s = shm;
    int k;
    for( k = 0; k < sizeof last; k++ )
        *s++ = last[k]; /* post fix */
    for (s = shm; *s != (char) NULL; s++)
        putchar(*s);
    putchar('\n');

    /** now remove the shared memory segment */
    shmctl(shmid, IPC_RMID, NULL);

    /** now remove the shared memory segment */
    shmctl(nhmid, IPC_RMID, NULL);

    return 0;
}

int process_b() {
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    char new[] = "I am Process B";

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 1;


    int nhmid;
    key_t int_key;
    char *nhm;
    char *n;
    int_key = 2;
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */


    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }


    if ((nhmid = shmget(int_key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((nhm = shmat(nhmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    /*
     * Now read what the server put in the memory.
     */
    while (!((void *) nhm == '1')){
        usleep(10);
        s = shm;
        n = nhm;

        if (*nhm == '1'){
            for(c = 0; c < sizeof new; c++ )
                *s++ = new[c]; /* post fix */
            *s = (char) NULL;
            *n = '2';
            break;
        }

    }


    return 0;
}

int process_c() {
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    char new[] = "I am Process C";

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 1;


    int nhmid;
    key_t int_key;
    char *nhm;
    char *n;
    int_key = 2;
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */


    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }


    if ((nhmid = shmget(int_key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
    if ((nhm = shmat(nhmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    /*
     * Now read what the server put in the memory.
     */

    while (!((void *) nhm == '2')) {
        usleep(10);
        s = shm;
        n = nhm;


        if (*nhm == '2') {
            for (c = 0; c < sizeof new; c++)
                *s++ = new[c]; /* post fix */
            *s = (char) NULL;
            *n = '3';
            break;
        }

    }


    return 0;
}