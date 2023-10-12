#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "./mypthread.h"
#include "./mypthread.c"

void* pow2(void* size){
    //printf("I MADE IT!!!!\n");
    //printf("x = %d\n", *(int*)size);
    int thing = 1;
    double thing2 = 2.0;
    double thing3 = 0.0;
    for(int i = 0; i < *(int*)size; i++){
        thing = thing * 2;
    }
    *(int*)size = thing;
    return size;
}

int main(int argc, char **argv) {
    mypthread_t tid1;
    mypthread_t tid2;
    mypthread_t tid3;

    int variable;
    int *ptr = &variable;
    *ptr = 10;

    int variable2;
    int *ptr2 = &variable2;
    *ptr2 = 20;

    int variable3;
    int *ptr3 = &variable3;
    *ptr3 = 30;
// 
    int* size_ret1 = (int*) malloc(sizeof(int));
    int* size_ret2 = (int*) malloc(sizeof(int));
    int* size_ret3 = (int*) malloc(sizeof(int));

    mypthread_create(&tid1, NULL, pow2, ptr);
    mypthread_create(&tid2, NULL, pow2, ptr2);
    mypthread_create(&tid3, NULL, pow2, ptr3);


    mypthread_join(tid1, (void**)&size_ret1);
    mypthread_join(tid2, (void**)&size_ret2);
    mypthread_join(tid3, (void**)&size_ret3);

    printf(" got value %d from thread %u\n", *size_ret1, tid1);
    printf(" got value %d from thread %u\n", *size_ret2, tid2);
    printf(" got value %d from thread %u\n", *size_ret3, tid3);
    
	return 0;
}


