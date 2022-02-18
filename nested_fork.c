//
// Created by masri on 2/17/2022.
//
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
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    char new[] = "I am Process A";
    char last[] = "Goodbye";
    key = 1;


    int nhmid;
    key_t int_key;
    char *nhm;
    char *n;
    int_key = 2;


// Process A

    if( (shmid = shmget(key, SHMSZ, IPC_CREAT | 0644)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    if( (shm = shmat(shmid, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    if((nhmid = shmget(int_key, SHMSZ, IPC_CREAT | 0644)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

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

    for (s = shm; *s != (char) NULL; s++)
        putchar(*s);
    putchar('\n');

    fork();
    fork();
    
// Process B
    char new_b[] = "I am Process B";

    if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if ((nhmid = shmget(int_key, SHMSZ, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }
  
    if ((nhm = shmat(nhmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    usleep(10);
    s = shm;
    n = nhm;

    if (*nhm == '1'){
        for(c = 0; c < sizeof new_b; c++ )
            *s++ = new_b[c]; /* post fix */
        *s = (char) NULL;
        *n = '2';

        for (s = shm; *s != (char) NULL; s++)
        putchar(*s);
        putchar('\n');
    }
    
// Process C
    char new_c[] = "I am Process C";

    if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
            perror("shmget");
            exit(1);
        }

        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
        }


        if ((nhmid = shmget(int_key, SHMSZ, 0644)) < 0) {
            perror("shmget");
            exit(1);
        }

        if ((nhm = shmat(nhmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
        }

        usleep(10);
        s = shm;
        n = nhm;


        if (*nhm == '2') {
            for (c = 0; c < sizeof new_c; c++)
                *s++ = new_c[c]; /* post fix */
            *s = (char) NULL;
            *n = '3';

            for (s = shm; *s != (char) NULL; s++)
            putchar(*s);
            putchar('\n');
        }

        
    // Goodbye

        if ((shmid = shmget(key, SHMSZ, 0644)) < 0) {
            perror("shmget");
            exit(1);
        }

        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
        }

        s = shm;
        int k;
        if (*nhm == '3') {
            for( k = 0; k < sizeof last; k++ )
                *s++ = last[k]; /* post fix */
            *s = (char) NULL;
            *n = '*';
            for (s = shm; *s != (char) NULL; s++)
                putchar(*s);
            putchar('\n');
        }


        return 0;
}