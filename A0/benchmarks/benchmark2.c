#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "../mypthread.h"

#define DEFAULT_THREAD_NUM 2
#define ARRAY_SIZE 5

/* Global variables */
pthread_mutex_t   mutex, counter_mutex, map_mutex, reduce_mutex;
int thread_num;
int* counter;
pthread_t *thread;
int *thread_counter;
int ar_parr[ARRAY_SIZE][ARRAY_SIZE];
int ar_verify[ARRAY_SIZE][ARRAY_SIZE];
int reduced_sum = 0;
int res = 0;

void executor(void *func, int arg){
	pthread_mutex_lock(&counter_mutex);
	pthread_create(&thread[*thread_counter], NULL, func, &arg);
	*thread_counter++;
	int thread_num = *thread_counter;
	pthread_mutex_unlock(&counter_mutex);
	pthread_join(thread[thread_num], NULL);
}

void map(int *pos){
	for(int i=0; i < ARRAY_SIZE; i++){
		pthread_mutex_lock(&map_mutex);
		ar_parr[*pos][i] = ar_parr[*pos][i] * 2;
		if(*pos == 1){
			printf("\nMM");
		}
		pthread_mutex_unlock(&map_mutex);
	}
}

void reduce(){
	for(int i=0; i < ARRAY_SIZE; i++){
		for(int i=0; i< ARRAY_SIZE; i++){
			pthread_mutex_lock(&reduce_mutex);
			reduced_sum += ar_parr[i];
			pthread_mutex_unlock(&reduce_mutex);
		}
		
	}
}

void verify() {
	// int i = 0;
	// res = 0;
	// for (i = 0; i < VECTOR_SIZE; i += 1) {
	// 	res += r[i] * s[i];	
	// }
	// printf("verified res is: %d\n", res);
}

void doNothing(){

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
	thread_counter = (int*)malloc(sizeof(int));
	// Thread Counter is 0 indexed
	*thread_counter = 0;
	// for (i = 0; i < thread_num; ++i)
	// 	counter[i] = i;

	// initialize pthread_t
	thread = (pthread_t*)malloc(thread_num*sizeof(pthread_t));

	// initialize data array
	for (int j = 0; j < ARRAY_SIZE; ++j) {
		for (int k = 0; k < ARRAY_SIZE; ++k) {
			ar_parr[j][k] = j + k;
		}
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&counter_mutex, NULL);
	pthread_mutex_init(&map_mutex, NULL);
	pthread_mutex_init(&reduce_mutex, NULL);

	int doNothingThreadCounter = 0;
	pthread_create(&thread[doNothingThreadCounter], NULL, &doNothing, thread_counter);

	struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);

	for(int i=0; i < ARRAY_SIZE; i++){
		executor(map, i);
		printf("\nM 1");
	}
	printf("\n M 3");
	pthread_join(thread[doNothingThreadCounter], NULL);
	printf("\n M 4");


	clock_gettime(CLOCK_REALTIME, &end);
        printf("running time: %lu micro-seconds\n", 
	       (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000);
	printf("res is: %d\n", res);
		for (int j = 0; j < ARRAY_SIZE; ++j) {
		for (int k = 0; k < ARRAY_SIZE; ++k) {
			printf("&d ",ar_parr[j][k]);
		}
		printf("\n");
	}

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&map_mutex);
	pthread_mutex_destroy(&reduce_mutex);

	verify();

	// Free memory on Heap
	free(thread);
	free(counter);
	return 0;
}
