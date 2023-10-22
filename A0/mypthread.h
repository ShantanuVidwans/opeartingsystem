// File:	mypthread_t.h

// List all group members' names:
// iLab machine tested on:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* in order to use the built-in Linux pthread library as a control for benchmarking, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>



/* add important states in a thread control block */

typedef uint mypthread_t;
typedef enum _t_pirority {
	LOW,
	MED,
	HIGH
}t_priority;

typedef enum t_state{
  NEW,
  RUNNING,
  WAITING,
  YIELDED,
  BLOCKING,
  TERMINATED,
}t_state;

typedef enum _mode {
	RR,
	PSJF,
	MLFQ
} mode;

typedef void* *func_ptr (void*);

typedef struct threadControlBlock
{
  mypthread_t tid; //id
  t_priority priority; //priority
  char* name;  //name
  void* t_retval; // return value
  func_ptr* func;
  void* arg;
  ucontext_t* t_context; // context
  t_state state; // state
  struct timespec start_init; // started init time
  struct timespec start_exec; // started execution time
  struct timespec start_blocked; // started blocked state time
  struct timespec start_wait; // started watiting time
  struct timespec total_exec; // total_execution time
  struct timespec total_blocked; // total ready time
  struct timespec total_wait; // total wait time
  mypthread_t parent_id; // parent thread
} tcb;

typedef struct _TCBNODE {
	tcb* tcb;
	struct _TCBNODE* next;
} tcb_node;

typedef struct _TCBQUEUE {
  tcb_node *front;
} tcb_stack;

/* mutex struct definition */
typedef struct _mypthread_mutex_t
{
  int flag; //id
  int guard; //lock/unlock
  int mypthread_t; //thread that owns it
	
}mypthread_mutex_t;

typedef struct _MUTEXNODE
{
  mypthread_mutex_t* mutex;
  struct _MUTEXNODE* next; 
	
}mutex_node;

typedef struct _MutexHandler {
	mutex_node* mutexQ;
	unsigned int mutex_size;

} MH;


// Feel free to add your own auxiliary data structures (linked list or stack etc...)
typedef struct _Scheduler{
   tcb* tcb;
   struct _TCBQUEUE* high;
   struct _TCBQUEUE* medium;
   struct _TCBQUEUE* low;
   struct _TCBQUEUE* ready;
   struct _TCBQUEUE* resource;
   struct _TCBQUEUE* wait;
   struct _TCBQUEUE* cleaner;
   tcb* current;
}TH;

void initializeMutexQ(MH* MQ){
  MQ->mutexQ = NULL;
  MQ->mutex_size = 1;
}

tcb_stack* createStack()
{
    tcb_stack* q
        = (tcb_stack*)malloc(sizeof(tcb_stack));
    q->front = NULL;
    return q;
}

void initializeTH(TH* scheduler)
{
  scheduler->high = createStack();
  scheduler->medium = createStack();
  scheduler->low = createStack();
  scheduler->ready = createStack();
  scheduler->cleaner = createStack();
  scheduler->wait= createStack();
  scheduler->resource = createStack();
 
}

void getTime(struct timespec time){
	clock_gettime(CLOCK_REALTIME, &time);
}

tcb* setupThread(ucontext_t* context, mypthread_t parent_id){
	tcb* t = (tcb*) malloc(sizeof(tcb));
	t->tid = rand();
	t->priority = HIGH;
	t->name = NULL;
	t->t_retval = NULL;
	t->t_context = context;
	t->state = NEW;
  t->parent_id = parent_id; // parent thread
  return t;
}

tcb_node* createTCBNode(tcb* tcb){
    tcb_node* new_node = (tcb_node*) malloc(sizeof(tcb_node));
    new_node->tcb = tcb;
    new_node->next = NULL;
    return new_node;
} 

void push(tcb* tcb, tcb_stack* q){
   
   tcb_node* temp = createTCBNode(tcb);

   q->front = temp;
}

tcb_node* pop(tcb_stack* q)
{
    // If stack is empty, return NULL.
    if (q->front == NULL)
        return NULL;
 
    // Store previous front and move front one node ahead
    tcb_node* temp = q->front;
 
    q->front = q->front->next;
 
    return temp;
}

tcb* peek(tcb_stack* q){
  if(q->front != NULL)
    return q->front->tcb;
  return NULL;
}



void swapStacks(tcb_stack* source, tcb_stack* destination, mypthread_t tid);


void printStack(tcb_stack* q){
   tcb_node *p = q->front;
   printf("\n[");

   //start from the beginning
   while(p != NULL) {
      printf(" %d ",p->tcb->tid);
      p = p->next;
   }
   printf("]\n");
}

/* Function Declarations: */

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);

/* current thread voluntarily surrenders its remaining runtime for other threads to use */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* wait for thread termination */
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
#define HIGH_EXEC_TIMEOUT 20
#define MEDIUM_EXEC_TIMEOUT 3700
#define LOW_EXEC_TIMEOUT 5000
#define T_STACK_SIZE 1048576
#define MAINTENANCE_THRESHOLD 2000
#define DISABLE_TIME 0
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
extern int mode_bit;
#endif

#endif


