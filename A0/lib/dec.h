#ifndef DECLARATIONS_H
#define DECLARATIONS_H

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

#endif