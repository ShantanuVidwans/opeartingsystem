#ifndef QUEUE_H
#define QUEUE_H

// Queue Code...................................................................................
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

tcb_node *createTCBNode(tcb *tcb) {
    tcb_node *new_node = (tcb_node *) malloc(sizeof(tcb_node));
    new_node->tcb = tcb;
    new_node->next = NULL;
    return new_node;
}

void enqueue(tcb_node *tcb, tcb_queue *q) {
    if (tcb == NULL)
        return;
    tcb->next = q->front;
    q->front = tcb;

}

int isEmpty(tcb_queue *q) {
    return (q->front == NULL);
}

tcb_node *dequeue(tcb_queue *q) {
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return NULL;

    // Store previous front and move front one node ahead
    tcb_node *temp = q->front;


    q->front = q->front->next;

    temp->next = NULL; //IMPORTANT

    return temp;
}

tcb *peek(tcb_queue *q) {
    if (q->front != NULL)
        return q->front->tcb;
    return NULL;
}


int transferQueue(tcb_queue *source, tcb_queue *destination) {
    // printf("transfering source PRE:");
    // printQueue(source);
    // printf("transfering destination PRE:");
    // printQueue(destination);
    while (!isEmpty(source)) {

        tcb_node *temp = dequeue(source);
        if (temp != NULL)
            enqueue(temp, destination);
    }

    // printf("transfering source POST:");
    // printQueue(source);
    // printf("transfering destination POST:");
    // printQueue(destination);
    return 0;
}

int insertAtEnd(tcb_node *tcb, tcb_queue *q) {
    tcb_node *p = q->front;

    // point it to old first node
    while (p->next != NULL)
        p = p->next;

    //point first to new first node
    p->next = tcb;

}

tcb_node *searchQueue(tcb_queue *q, mypthread_t tid) {
    tcb_node *temp = q->front;

    if (q->front == NULL)
        return NULL;

    while (temp != NULL) {
        if (temp->tcb->tid == tid) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

tcb_node *searchQueueAndRemove(tcb_queue *q, mypthread_t tid) {

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

    if (temp == NULL) {
        return NULL;
    }

    prev->next = temp->next;

    return temp;
}

int swapQueues(tcb_queue *source, tcb_queue *destination, mypthread_t tid) {
    tcb_node *node = searchQueueAndRemove(source, tid);
    if (node == NULL) {
        printf("Node with id %u", tid);
        return -1;
    }
    insertAtEnd(node, destination);
    return 0;
}

// Some code ........................................
tcb_queue *createQueue(char *name) {
    tcb_queue *q
            = (tcb_queue *) malloc(sizeof(tcb_queue));
    q->name = name;
    q->front = NULL;
    return q;
}

#endif