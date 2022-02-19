#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#define SHMSZ     27


int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}


// (avg.x, avg_proc.c and ravg.
int main(){
    int arr[11] = {9,2,3,4,5,6,7,8,9,12,3};
    int size = sizeof(arr) / sizeof(arr[0]);
    qsort (arr, size, sizeof(*arr), comp);
    printf("%d\n",arr[0]);

    // int i;
    // for (i = 0; i < size; i++){
    //     printf("%d\n", arr[i]);
    // }
    return 0;

}
