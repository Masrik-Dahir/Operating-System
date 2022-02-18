/*
 * shm-client - client program to demonstrate shared memory.
 * shm-client - not sure of the origin of these code segments.
 * possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
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
    char new[] = "Process C";

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 1;


    int nhmid;
    key_t string_key;
    char *nhm;
    char *n;
    string_key = 2;
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


    if ((nhmid = shmget(string_key, SHMSZ, 0644)) < 0) {
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
    s = shm;
    n = nhm;
    for(c = 0; c < sizeof new; c++ )
        *s++ = new[c]; /* post fix */
    *s = (char) NULL;
    *n = '2';


    for (s = shm; *s != (char) NULL; s++)
        putchar(*s);
    putchar('\n');




    /*
     * Finally, change the first character of the
     * segment to '*', indicating we have read
     * the segment.
     */
//    *shm = '*';
    return 0;
}

main();