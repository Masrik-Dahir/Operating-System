/*
 * Masrik Dahir
 * Date: 02/20/2022
 * The process_b is equivalent of a client and it waits for process_a and then it executes
 * process_a, process_b, or process_c cab be run in 
 * any order or any time. However, process_a executes
 * first, then process_b executes, and then process_c executes
 * After process_c input 3 in shared integer memory, process_a prints "Goodbye"
 * 
 * 
 * Usage Guide (Process B in new terminal)
 * (1) gcc process_b.c -o b
 * (2) ./b
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#define SHMSZ     27
int
main() {
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
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
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


    if ((nhmid = shmget(int_key, SHMSZ, 0666)) < 0) {
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

