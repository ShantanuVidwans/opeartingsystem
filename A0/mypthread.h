// File:	mypthread_t.h

// List all group members' names:
// iLab machine tested on:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* in order to use the built-in Linux pthread library as a control for benchmarking, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include "lib/queue.h"
#include "lib/dec.h"
#include "lib/mutex.h"


/* add important states in a thread control block */



// Feel free to add your own auxiliary data structures (linked list or queue etc...)
typedef struct _Scheduler{
   tcb_node* s_tcb_node;
   struct _TCBQUEUE* running;
   struct _TCBQUEUE* medium;
   struct _TCBQUEUE* low;
   struct _TCBQUEUE* ready;
   struct _TCBQUEUE* resource;
   struct _TCBQUEUE* blocked;
   struct _TCBQUEUE* terminated;
   tcb_node* current;
}TH;

/* include lib header files that you need here: */

void initializeTH(TH* scheduler)
{  
  scheduler->running = createQueue("running");
  scheduler->medium = createQueue("medium");
  scheduler->low = createQueue("low");
  scheduler->ready = createQueue("ready");
  scheduler->terminated = createQueue("terminated");
  scheduler->blocked= createQueue("blocked");
  scheduler->resource = createQueue("resource");
 
}

void getTime(struct timespec time){
	clock_gettime(CLOCK_REALTIME, &time);
}

tcb* setupThread(ucontext_t* context, mypthread_t join_id){
	tcb* t = (tcb*) malloc(sizeof(tcb));
	t->tid = rand();
	t->priority = HIGH;
	t->name = NULL;
	t->t_retval = NULL;
	t->t_context = context;
	t->state = RUNNING;
  t->join_id = join_id; // parent thread
  return t;
}



/* Function Declarations: */

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);

/* current thread voluntarily surrenders its remaining runtime for other threads to use */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* blocked for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr);

/* initialize a mutex */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire a mutex (lock) */
int mypthread_mutex_lock(mypthread_mutex_t *mutex);

/* release a mutex (unlock) */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

/* destroy a mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);






#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#define HIGH_EXEC_TIMEOUT 30
#define MEDIUM_EXEC_TIMEOUT 60
#define LOW_EXEC_TIMEOUT 90
#define T_STACK_SIZE 1048576
extern unsigned int s_tid;  //scheduler id
extern TH* MTH; // scheduler
extern MH* MQ;
extern ucontext_t ctx_main; // main context
extern ucontext_t ctx_handler; // context handler
extern mode schedMode; //Which scheduler we use
extern int isSchedulerNotStarted;
extern int isMutexQNotAllocated;
extern struct sigaction act_timer;
extern struct itimerval timer;
extern int load_balance;
#endif

#endif


