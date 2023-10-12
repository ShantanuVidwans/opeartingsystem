#ifndef QUEUE_H
#define QUEUE_H
#include "types.h"



tcb_node* createTCBNode(tcb* tcb);
void enqueue(tcb_node* tcb, tcb_queue* q);
tcb_node* dequeue(tcb_queue* q);
tcb* peek(tcb_queue* q);
void printQueue(tcb_queue* q);
tcb_node* searchQueue(tcb_queue* q, mypthread_t tid);
tcb_node* searchQueueAndRemove(tcb_queue* q, mypthread_t tid);
int swapQueues(tcb_queue* source, tcb_queue* destination, mypthread_t tid);

#endif