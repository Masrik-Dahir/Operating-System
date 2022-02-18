
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>

#endif

/*
 * shm-server - not sure of the origin of code segment.
 * Old source, not sure of the origin
 *  possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#define SHMSZ     27
int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    key = 2229;


    char p[] = "Masrik Dahir";
    int nhmid;
    key_t string_key;
    char *nhm;
    char *n;
    string_key = 2230;

    /*
     * We'll name our shared memory segment
     * "5678".
     */

    /*
     * Create the segment.
     */
    if( (shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0 )
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


    if( (nhmid = shmget(string_key, SHMSZ, IPC_CREAT | 0666)) < 0 )
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


    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;
    n = nhm;

//    for( c = 'a'; c <= 'z'; c++ )
//        *s++ = c; /* post fix */

    // Newly added
    *s = '1';

    for( c = i; c <= 4; c++ ){
        *n++ = p[c]; /* post fix */
    }

//    *s = (char) NULL;

//    *n = (char) NULL;


    /*
     * Finally, we wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    putchar(*s);
    putchar(*n);

//    For the string


    while( *shm != '*' )
        sleep(1);
    return 0;
}

main();