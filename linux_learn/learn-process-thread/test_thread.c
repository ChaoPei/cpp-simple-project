// must link pthread library 
// gcc -o run test_thread.c -l pthread

#include <stdio.h>
#include <pthread.h>

static int run = 1;
static int retvalue;

void* child_pthread(void *arg) {
	// 因为是指针数据，所以在外部改变也会导致running改变，可以从外部控制循环结束
	int *running = arg;
	printf("child thread init: %d success!\n", *running);
	while(*running){
		printf("child running: running = %d\n", *running);
		usleep(1);
	}

	printf("child exit: running = %d\n", *running);

	retvalue = 8;
	pthread_exit((void*) &retvalue);
}

int main(void) {

	pthread_t pt;
	int ret = -1;
	int times = 3;
	int i = 0;
	int *ret_join = NULL;

	ret = pthread_create(&pt, NULL, (void*)child_pthread, &run);

	if(ret != 0) {
		printf("create child thread failure!\n");
		return 1;
	}

	usleep(1);

	for(; i < times; ++i) {
		printf("main thread running!\n");
		usleep(1);
	}
	
	// control thread exit
	run = 0;
	pthread_join(pt, (void*)&ret_join);
	printf("child thread return: %d\n", *ret_join);
	return 0;
}
	
