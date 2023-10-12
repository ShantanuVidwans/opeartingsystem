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

typedef uint mypthread_t;

typedef enum _t_pirority {
    LOW,
    MED,
    HIGH
}t_priority;

typedef enum t_state{
  RUNNING,
  WAITING,
  YIELDED,
  BLOCKED,
  TERMINATED,
}t_state;

typedef enum _mode {
    RR,
    PSJF,
    MLFQ
} mode;


typedef void* (*funcptr)(void*);
typedef struct threadControlBlock
{
  mypthread_t tid; //id
  t_priority priority; //priority
  char* name;  //name
  void* t_retval; // return value
  funcptr func_ptr;
  void* arg;
  ucontext_t* t_context; // context
  t_state state; // state
  struct timespec start_exec; // started execution time
  struct timespec total_exec; // total_execution time
  mypthread_t join_id; // parent thread
} tcb;

typedef struct _TCBNODE {
    tcb* tcb;
    struct _TCBNODE* next;
} tcb_node;

typedef struct _TCBQUEUE {
   char* name;
  struct _TCBNODE* front;
} tcb_queue;


/* add important states in a thread control block */

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


void initializeMutexQ(MH* MQ){
  MQ->mutexQ = NULL;
  MQ->mutex_size = 1;
}

tcb_queue* createQueue(char* name)
{
    tcb_queue* q
        = (tcb_queue*)malloc(sizeof(tcb_queue));
    q->name = name;
    q->front = NULL;
    return q;
}

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


//Queue Code

void printQueue(tcb_queue* q){
   printf("\nprinting queue %s:", q->name);
   tcb_node *p = q->front;
   printf("[");

   //start from the beginning
   while(p != NULL) {
      printf(" %d ",p->tcb->tid);
      p = p->next;
   }
   printf("]\n");
   return;
}

tcb_node* createTCBNode(tcb* tcb){
    tcb_node* new_node = (tcb_node*) malloc(sizeof(tcb_node));
    new_node->tcb = tcb;
    new_node->next = NULL;
    return new_node;
} 

void enqueue(tcb_node* tcb, tcb_queue* q){
    if(tcb == NULL)
        return;
    tcb->next = q->front;
    q->front = tcb;

}

int isEmpty(tcb_queue* q) {
    return (q->front == NULL);
}

tcb_node* dequeue(tcb_queue* q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return NULL;
 
    // Store previous front and move front one node ahead
    tcb_node* temp = q->front;

 
    q->front = q->front->next;

    temp->next = NULL; //IMPORTANT 
 
    return temp;
}

tcb* peek(tcb_queue* q){
  if(q->front != NULL)
    return q->front->tcb;
  return NULL;
}



int transferQueue(tcb_queue* source, tcb_queue* destination){
    // printf("transfering source PRE:");
    // printQueue(source);
    // printf("transfering destination PRE:");
    // printQueue(destination);
    while(!isEmpty(source)){
        
        tcb_node* temp = dequeue(source);
            if(temp != NULL)
                enqueue(temp, destination);    
        }

    // printf("transfering source POST:");
    // printQueue(source);
    // printf("transfering destination POST:");
    // printQueue(destination);
    return 0;
}

int insertAtEnd(tcb_node* tcb, tcb_queue* q){
    tcb_node* p = q->front;

   // point it to old first node
   while(p->next != NULL)
      p = p->next;

    //point first to new first node
    p->next = tcb;

}

tcb_node* searchQueue(tcb_queue* q, mypthread_t tid)
{
   tcb_node *temp = q->front;
    
    if (q->front == NULL)
        return NULL;
 
    while(temp != NULL){
        if(temp->tcb->tid == tid){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

tcb_node* searchQueueAndRemove(tcb_queue* q, mypthread_t tid)
{

     tcb_node *temp = q->front;
     tcb_node *prev = NULL;

    if (q->front == NULL)
        return NULL;

    if (temp != NULL && temp->tcb->tid == tid) {
      q->front = temp->next;
      return temp;
    }

    while (temp != NULL && temp->tcb->tid != tid) {
      prev = temp;
      temp = temp->next;
   }

   if (temp == NULL){
    return NULL;
   }

   prev->next=temp->next;

   return temp;
}

int swapQueues(tcb_queue* source, tcb_queue* destination, mypthread_t tid){
    tcb_node* node = searchQueueAndRemove(source, tid);
    if (node == NULL){
        printf("Node with id %u", tid);
        return -1;
    }
    insertAtEnd(node, destination);
    return 0;
}






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


