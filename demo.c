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



    int p1,p2,p3,i;
    int left,leftPid;
    int right;  
    left=fork();
    right=fork();
    for(i=0;i<=2;i++){
        left=fork();
        leftPid=left; 

        if(left==0) //use break statement for left child since we want it to be kicked out and not execute anything!
        {
            // printf("\nleft Child Process. Level: %d | myPID: %d | myParent: %d\n",i,getpid(),getppid());
            break; // break statement has to used here necessarily or else left child  will keep on making left childs
        }         
        else if(left>0) //this is executed by parent
        {
            right=fork(); //parent forks a right child

            if (right==0) //this is executed by right child
            {
                printf("Process %d\n", i);
            } 
            else if(right>0) //executed by parent
            {
                printf("Process Parent\n");
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
                break; //again use break to kick out parent since now this parent has no work to do and break statement has to used here necessarily or else parent will keep on making childs-left and right
            }
        }    
}
}