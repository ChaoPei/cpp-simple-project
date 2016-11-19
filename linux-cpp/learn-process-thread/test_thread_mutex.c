#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

void *producer(void *arg);
void *consumer(void *arg);

// buffers number
int buffer_num = 0;
// 线程互斥区
pthread_mutex_t mutex;
// control thread run and exit
int running = 1;


int main(void) {
	
	pthread_t producer_pt;
	pthread_t consumer_pt;

	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&producer_pt, NULL, (void*)producer, NULL);
	pthread_create(&consumer_pt, NULL, (void*)consumer, NULL);
	
	usleep(100);
	running = 0;
	pthread_join(consumer_pt, NULL);
	pthread_join(producer_pt, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}


void *producer(void *arg) {

	while(running) {
		// 进入互斥区, 将其锁定
		pthread_mutex_lock(&mutex);
		buffer_num++;
		printf("produce + buffer number: %d\n", buffer_num);
		// 解锁
		pthread_mutex_unlock(&mutex);
	}
}

void *consumer(void *arg) {
	while(running) {
	//	usleep(1);
		pthread_mutex_lock(&mutex);
		--buffer_num;
		printf("consume - buffer number: %d\n", buffer_num);
		pthread_mutex_unlock(&mutex);
	}
}

