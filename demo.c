//
// Created by masri on 2/17/2022.
//
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <stdbool.h>
#include <unistd.h>
int main(){
    char new = 'w';
    char *nhm = new;
    if ((void *) nhm == 'w'){
        printf("%c", (void *) nhm);
    }




    return 0;
}
