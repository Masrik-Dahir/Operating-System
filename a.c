#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
/*
 * shm-server - not sure of the origin of code segment.
 * Old source, not sure of the origin
 *  possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#define SHMSZ     27

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>

#endif


int
main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    char new[] = "Process A";

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
    /*
     * Now put some things into the memory for the
     * other process to read.
     */


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



    while( *shm != '*' )
//        sleep(1);
        if (*nhm == '2'){
            for (s = shm; *s != (char) NULL; s++)
                putchar(*s);
            putchar('\n');
            *nhm = '*';
        }
        if (*nhm == '3'){
            for (s = shm; *s != (char) NULL; s++)
                putchar(*s);
            putchar('\n');
            *nhm = '&';
        }

        if (*nhm == '&'){
            putchar('a');
//            char okay[] = "Goodbye";
//            for( c = 0; c < sizeof okay; c++ )
//                *s++ = okay[c]; /* post fix */
//            *s = (char) NULL;
//
//            for (s = shm; *s != (char) NULL; s++)
//                putchar(*s);
//            putchar('\n');
//            *shm = '!';
        }


    return 0;
}