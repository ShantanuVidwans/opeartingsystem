// File:	mypthread.c

// List all group members' names:
// iLab machine tested on:

#include "mypthread.h"
void setTimer(long int time_milli);
//Global Vars
unsigned int s_tid = 0;  //scheduler id
TH* MTH = NULL; // scheduler
MH* MutexQ = NULL; // mutex q
ucontext_t ctx_main; // main context
ucontext_t ctx_handler; // context handler
mode schedMode = RR; //Which scheduler we use
int isSchedulerNotStarted = 1;
int isMutexQNotAllocated = 1;
struct sigaction act_timer;
struct itimerval timer;
int mode_bit = 0;

void* executeThread() {
	//Call Exit here soon
	tcb* current_thread = MTH->current;
	printf("Oy shoe I made it %i\n", current_thread->tid);
	MTH->current->t_retval = current_thread->func(MTH->current->arg);
	MTH->current->state = TERMINATED;
	printf("got this back boi %d \n", MTH->current->t_retval );
	while(1);
}


static void schedule(int signum){
	setTimer(DISABLE_TIME);
	if (MTH->current!=NULL){
		getcontext(MTH->current->t_context);
		printf("%d\n", MTH->current->state );
	}
	printf("Entering Schedule Mode\n");
	if (__sync_add_and_fetch(&mode_bit,1) != 1) {
		return;
	}
	
	if(signum == SIGALRM){
		printf("I AM HERE BECAUSE I WAS INTERRUTPED\n");
	}



	if (__sync_add_and_fetch(&mode_bit,-1) != 0) {
		printf("Unable to leave scheduler mode\n");
		exit(1);
	}
	printf("Leaving scheduler mode\n");
	setTimer(HIGH_EXEC_TIMEOUT);
	printf("Next up is %li \n", peek(MTH->ready)->tid);
	MTH->current=peek(MTH->ready);
	setcontext(peek(MTH->ready)->t_context);
}


void setTimer(long int time_milli){
	printf("Setting Timer to %li \n", time_milli);
    act_timer.sa_flags=0;
    act_timer.sa_handler=&schedule;
    sigemptyset(&act_timer.sa_mask);
    sigaddset(&act_timer.sa_mask, SIGALRM);
    sigaction(SIGALRM, &act_timer, NULL); 
    
    timer.it_value.tv_sec = time_milli;
 	timer.it_value.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
    	printf("Unable to set time in for MTH\n");
    	exit(1);
        }
    printStack(MTH->ready);
    printf("Successfully set timer to %li \n", time_milli);

}




//Starts MTH Context and method
tcb* initTH(TH* MTH) {
	printf("Setting up main thread \n");
	return setupThread(&ctx_main, s_tid);//At this point the MTH is setup and we have the main thread;
}

void initMutexQ(MH* MutexQ) {
	MutexQ = (MH*) malloc(sizeof(MH));
	initializeMutexQ(MutexQ);

}

static void sched_RR() {
	
	if(isSchedulerNotStarted) {
		tcb* main_thread = initTH(MTH);
		MTH->tcb = setupThread(&ctx_handler, main_thread->tid);
		s_tid = MTH->tcb->tid;
		printf("main_thread created with id %d\n", main_thread->tid);
		printf("scheduler_thread created with id %d\n", s_tid); 
		isSchedulerNotStarted = 0;
		push(main_thread, MTH->ready);
		printStack(MTH->ready);
		schedule(-1);
            
	}
	
	//insertatbegin(mainThread, MTH->ready);
	

	while(1){ 
		/// Check Ready Queue
		//swapcontext(&ctx_handler, mainThread->t_context);
		}
	// if(MTH->ready != NULL){
	//  printf("SWAPPING to THREAD %d\n",MTH->ready->tcb->tid);
	//  swapcontext(&ctx_handler,MTH->ready->tcb->t_context);
	// }

	//join on main thread


}

void setupSchedulerContext(){
	getcontext(&ctx_handler);
 	ctx_handler.uc_link=0;
 	ctx_handler.uc_stack.ss_sp=malloc(T_STACK_SIZE);
 	ctx_handler.uc_stack.ss_size=T_STACK_SIZE;
 	ctx_handler.uc_stack.ss_flags=0;
 	return makecontext(&ctx_handler, (void*)&sched_RR, 0);
}


static int startScheduler() {
	if (MTH == NULL) {
		MTH = (TH*) malloc(sizeof(TH));
		initializeTH(MTH); //Setup the MTH's queues
		setupSchedulerContext(); //setup scheduler context
		swapcontext(&ctx_main,&ctx_handler);
		return 1;
	}
	return -1;
}

int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg)
{
	if(isSchedulerNotStarted) {
		printf("Setting Up\n");
		startScheduler();
		printf("Done Setting Up\n");
		isSchedulerNotStarted = 0;
	}	
	printf("MTH HAS ID %d\n", s_tid);


	ucontext_t* thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
	getcontext(thread_context);
 	
 	if ((thread_context->uc_stack.ss_sp = (char *) malloc(T_STACK_SIZE)) != NULL) {
		thread_context->uc_link=0;
 		thread_context->uc_stack.ss_size=T_STACK_SIZE;
 		thread_context->uc_stack.ss_flags=0;
 		
 		makecontext(thread_context, (void*)&executeThread,0);

		tcb* new_thread = setupThread(thread_context, s_tid);
		new_thread->func = (void (*)(void))function;
		new_thread->arg = arg;
		push(new_thread, MTH->ready);

		*thread = new_thread->tid;
		printf("Making Thread with Id %d\n",new_thread->tid);
		return new_thread->tid;
	}

	
	return -1;
};

int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
	// Set OS BIT
	if(isMutexQNotAllocated) {
		initMutexQ(MutexQ);
		isMutexQNotAllocated = 0;
	}


	return 0;
};


