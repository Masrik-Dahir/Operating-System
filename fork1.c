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

    int n1 = fork();
    int n2 = fork();
    int val;

    
    if (n1 > 0 && n2 > 0){
        val = '2';
        // printf("Process 1\n");
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

        
    }

    if (n1 > 0 && n2 == 0){
        val = '1';
        // printf("Process 2\n");
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

        s = shm;
        n = nhm;

        for(c = 0; c < sizeof new_b; c++ )
            *s++ = new_b[c]; /* post fix */
        *s = (char) NULL;
        *n = '2';

        
        
    }

    if (n1 == 0 && n2 > 0){
        val = '1';
        // printf("Process 3\n");
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

        usleep(20);
        s = shm;
        n = nhm;


        for (c = 0; c < sizeof new_c; c++)
            *s++ = new_c[c]; /* post fix */
        *s = (char) NULL;
        *n = '3';      

    }
    

    if (n1 == 0 && n2 == 0){
        val = 0;
        // printf("Process 4\n");

    }

    for (s = shm; *s != (char) NULL; s++)
            putchar(*s);
        // putchar(' ');
        // putchar(val);
        putchar('\n');
    return 0;
}