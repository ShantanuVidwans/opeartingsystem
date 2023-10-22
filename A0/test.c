#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "./mypthread.h"
#include "./mypthread.c"

#define DEFAULT_THREAD_NUM 2
#define VECTOR_SIZE 300000

/* Global variables */
pthread_mutex_t mutex;
int thread_num;
int* counter;
pthread_t *thread;
int r[VECTOR_SIZE];
int s[VECTOR_SIZE];
int res = 0;

/* A CPU-bound task to do vector multiplication */
void* vector_multiply(void* arg) {
    int i = 0;
    int n = *((int*) arg);
    
    for (i = n; i < VECTOR_SIZE; i += thread_num) {
        pthread_mutex_lock(&mutex);
        res += r[i] * s[i];
        pthread_mutex_unlock(&mutex);   
    }

    pthread_exit(NULL);
}

void verify() {
    int i = 0;
    res = 0;
    for (i = 0; i < VECTOR_SIZE; i += 1) {
        res += r[i] * s[i]; 
    }
    printf("verified res is: %d\n", res);
}

int main(int argc, char **argv) {
    
    int i = 0;

    if (argc == 1) {
        thread_num = DEFAULT_THREAD_NUM;
    } else {
        if (argv[1] < 1) {
            printf("enter a valid thread number\n");
            return 0;
        } else {
            thread_num = atoi(argv[1]);
        }
    }

    // initialize counter
    counter = (int*)malloc(thread_num*sizeof(int));
    for (i = 0; i < thread_num; ++i)
        counter[i] = i;

    // initialize pthread_t
    thread = (pthread_t*)malloc(thread_num*sizeof(pthread_t));

    // initialize data array
    for (i = 0; i < VECTOR_SIZE; ++i) {
        r[i] = i;
        s[i] = i;
    }

    pthread_mutex_init(&mutex, NULL);

    struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);

    for (i = 0; i < thread_num; ++i)
        pthread_create(&thread[i], NULL, &vector_multiply, &counter[i]);

    for (i = 0; i < thread_num; ++i)
        pthread_join(thread[i], NULL);

    clock_gettime(CLOCK_REALTIME, &end);
        printf("running time: %lu micro-seconds\n", 
           (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000);
    printf("res is: %d\n", res);

    pthread_mutex_destroy(&mutex);
    verify();

    // Free memory on Heap
    free(thread);
    free(counter);
    return 0;
}







// #include <stdio.h>
// #include <unistd.h>
// #include <pthread.h>
// #include "./mypthread.h"
// #include "./mypthread.c"


// int fib(int i){
//     if (i == 0){
//         return 0;
//     }
//     if (i == 1){
//         return 1;
//     }
    
//     return fib(i-1) + fib(i-2);

// }

// void vector_multiply(void* arg) {
//     int i = 0;
//     int n = *((int*) arg);
    
//     for (i = n; i < VECTOR_SIZE; i += thread_num) {
//         pthread_mutex_lock(&mutex);
//         res += r[i] * s[i];
//         pthread_mutex_unlock(&mutex);   
//     }

//     pthread_exit(NULL);
// }

// pthread_t *thread;
// int main(int argc, char **argv) {
//     mypthread_mutex_t   mutex;
//     mypthread_t tid1;
//     mypthread_t tid2;
//     mypthread_t tid3;

//     int variable;
//     int *ptr = &variable;
//     *ptr = 1;

//     int variable2;
//     int *ptr2 = &variable2;
//     *ptr2 = 10;

//     int variable3;
//     int *ptr3 = &variable3;
//     *ptr3 = 30;
//     pthread_mutex_init(&mutex, NULL);
//     int* size_ret1 = (int*) malloc(sizeof(int));
//     int* size_ret2 = (int*) malloc(sizeof(int));
//     int* size_ret3 = (int*) malloc(sizeof(int));
//     int thread_num = 10;
//     int i = 0;
//     thread = (mypthread_t*)malloc(thread_num*sizeof(mypthread_t));
//     for (i = 0; i < thread_num; i++){
        
//         mypthread_create(&thread[i],  NULL, vector_multiply, ptr3);
//         printf("Created %u\n", thread[i]);
        
//     }

//     for (i = 0; i < thread_num; i++){
//         mypthread_join(thread[i], NULL);
//         printf(" got value from thread %u\n", thread[i]);
//     }

//     printf("Queues\n");
//     printQueue(MTH->ready);
//     printQueue(MTH->running);
//     printQueue(MTH->blocked);
//     printQueue(MTH->terminated);
    
//     printf("\n____________________DONE___________________\n");
// }


