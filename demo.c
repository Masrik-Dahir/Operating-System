#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#define SHMSZ     27

int main(){
    process_a();
    int left = fork();
    int leftPID = left;
    int right;

    int i;
    
    if(left>0){
        process_b();
        right=fork();
        if(left>0){
            process_c();
        }
    }  

    return 0;
}