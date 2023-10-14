#ifndef MUTEX_H
#define MUTEX_H


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


void initializeMutexQ(MH* MQ){
  MQ->mutexQ = NULL;
  MQ->mutex_size = 1;
}



#endif