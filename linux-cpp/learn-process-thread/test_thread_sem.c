#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void *producer(void *arg);
void *consumer(void *arg);

// 信号量
sem_t sem;

// control thread run and exit
int running = 1;


int main(void) {
	
	pthread_t producer_pt;
	pthread_t consumer_pt;
	
	// init sem
	int sem_value = 1;
	sem_init(&sem, 0, sem_value);
	
	pthread_create(&producer_pt, NULL, (void*)producer, NULL);
	pthread_create(&consumer_pt, NULL, (void*)consumer, NULL);
	
	usleep(100);
	running = 0;
	pthread_join(consumer_pt, NULL);
	pthread_join(producer_pt, NULL);

	// destroy sem
	sem_destroy(&sem);

	return 0;
}


void *producer(void *arg) {
	
	int semval = 0;
	while(running) {
		usleep(1);
		sem_post(&sem);		// V操作, 释放资源, 信号量增加
		sem_getvalue(&sem, &semval);

		printf("produce + sem_value: %d\n", semval);
	}
}

void *consumer(void *arg) {
	int semval = 0;
	while(running) {
		usleep(1);
		sem_wait(&sem);		// P操作, 使用资源, 信号量减少
		sem_getvalue(&sem, &semval);

		printf("consumer - sem_value: %d\n", semval);
	}
}


