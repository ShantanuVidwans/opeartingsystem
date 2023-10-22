#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "./mypthread.h"
#include "./mypthread.c"
// unsigned int tid = 5;
// MTH* Master;
// ucontext_t* ctx_main;

// typedef struct _JUNK {
//     int x;
//     char y;
// }t_struct;

void* t1(void* size){
    printf("I MADE IT!!!!\n");
    printf("x = %d\n", *(int*)size);
    return 4;
}

int main(int argc, char **argv) {
    mypthread_t tid1;
    int variable;
    int *ptr = &variable;
    *ptr = 20;

    mypthread_create(&tid1,NULL, t1, ptr);
    printf("THREAD ID = %d\n",tid1);

    while(1);
    
	return 0;
}
