#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "./mypthread.h"
#include "./mypthread.c"

#define DEFAULT_THREAD_NUM 2
#define VECTOR_SIZE 30000

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


// tcb *setupThreadTest(double total_exec)
// {
//     tcb *t = (tcb *)malloc(sizeof(tcb));
//     t->tid = rand();
//     t->priority = HIGH;
//     t->name = NULL;
//     t->t_retval = NULL;
//     t->t_context = NULL;
//     t->start_exec = clock();
//     t->total_exec = total_exec;
//     t->state = RUNNING;
//     t->join_id = 0; // parent thread
//     return t;
// }

// tcb *thread1;
// tcb *thread2;
// tcb *thread3;
// tcb *thread4;
// tcb *thread5;
// tcb *thread6;
// tcb *thread7;
// tcb *thread8;
// int main(int argc, char **argv) {
//     struct _TCBQUEUE* ready = createQueue("Ready Queue");  
//     struct _TCBQUEUE* running = createQueue("Running queue");

//     // //Test 1;
//     // printf("|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|\n");
//     // thread8 = setupThreadTest(0);
//     // enqueue(createTCBNode(thread8), ready);
//     // printf("Before Transfer SINGLE ITEM TO EMPTY[\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("---------------------------------\n");
//     // transferQueueSJF(ready, running);
//     // printf("---------------------------------\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("After Transfer SINGLE ITEM TO EMPTy]\n");
//     // printf("|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|\n");

//     // //Test 2
//     // printf("|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|\n");
//     // thread1 = setupThreadTest(0);
//     // thread2 = setupThreadTest(5);
//     // thread3 = setupThreadTest(0);
//     // thread4 = setupThreadTest(2);
//     // thread5 = setupThreadTest(0);
//     // thread6 = setupThreadTest(1);
//     // thread7 = setupThreadTest(0);

//     // enqueue(createTCBNode(thread1), ready);
//     // enqueue(createTCBNode(thread2), ready);
//     // enqueue(createTCBNode(thread3), ready);
//     // enqueue(createTCBNode(thread4), ready);
//     // enqueue(createTCBNode(thread5), ready);
//     // enqueue(createTCBNode(thread6), ready);
//     // enqueue(createTCBNode(thread7), ready);
//     // printf("Before Transfer FULL OUT OF ORDER TO EMPTY[\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("---------------------------------\n");
//     // transferQueueSJF(ready, running);
//     // printf("---------------------------------\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("After Transfer FULL OUT OF ORDER TO EMPTY]\n");
//     // printf("|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|\n");


//     //Test 3
//     // printf("|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|\n");
//     // thread1 = setupThreadTest(0);
//     // thread2 = setupThreadTest(5);
//     // thread3 = setupThreadTest(0);
//     // thread4 = setupThreadTest(2);
//     // thread5 = setupThreadTest(0);
//     // thread6 = setupThreadTest(1);
//     // thread7 = setupThreadTest(0);

//     // enqueue(createTCBNode(thread1), ready);
//     // enqueue(createTCBNode(thread2), ready);
//     // enqueue(createTCBNode(thread3), ready);
//     // enqueue(createTCBNode(thread4), running);
//     // enqueue(createTCBNode(thread5), ready);
//     // enqueue(createTCBNode(thread6), running);
//     // enqueue(createTCBNode(thread7), ready);
//     // printf("Before Transfer FULL OUT OF ORDER TO ORDERED[\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("---------------------------------\n");
//     // transferQueueSJF(ready, running);
//     // printf("---------------------------------\n");
//     // printQueueTime(ready);
//     // printQueueTime(running);
//     // printf("After Transfer FULL OUT OF ORDER TO ORDERED]\n");
//     // printf("|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|\n");



//       //Test 3
//     printf("|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|\n");
//     thread1 = setupThreadTest(0);
//     thread2 = setupThreadTest(5);
//     thread3 = setupThreadTest(0);
//     thread4 = setupThreadTest(2);
//     thread5 = setupThreadTest(0);
//     thread6 = setupThreadTest(1);
//     thread7 = setupThreadTest(0);

//     enqueue(createTCBNode(thread1), ready);
//     enqueue(createTCBNode(thread4), running);
//     enqueue(createTCBNode(thread5), ready);
//     enqueue(createTCBNode(thread6), running);
//     enqueue(createTCBNode(thread7), ready);
//     printf("Before Transfer FULL OUT OF ORDER TO ORDERED[\n");
//     printQueueTime(ready);
//     printQueueTime(running);
//     printf("---------------------------------\n");
//     transferQueueSJF(ready, running);
//     printf("---------------------------------\n");
//     printQueueTime(ready);
//     printQueueTime(running);
//     printf("After Transfer FULL OUT OF ORDER TO ORDERED]\n");
//     enqueue(createTCBNode(thread2), ready);
//     enqueue(createTCBNode(thread3), ready);
//     printf("Before Transfer PARTIAL OF ORDER TO ORDERED[\n");
//     printQueueTime(ready);
//     printQueueTime(running);
//     printf("---------------------------------\n");
//     transferQueueSJF(ready, running);
//     printf("---------------------------------\n");
//     printQueueTime(ready);
//     printQueueTime(running);
//     printf("After Transfer PARTIAL OF ORDER TO ORDERED]\n");
//     printf("|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|\n");




//     printf("\n____________________DONE___________________\n");
// }


