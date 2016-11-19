#include <stdio.h>
#include <signal.h>
#include <sys/types.h>


typedef void(*signal_handler)(int);

// only this file exist funciton
// signal handle
static void sig_handle(int signo){
	if(SIGSTOP == signo){
		printf("receive stop signal.\n");
	}
	else if(SIGKILL == signo){
		printf("receive kill signal.\n");
	}
	else{
		printf("receive %d signal.\n", signo);
	}

	return;
}


int main(void) {
	signal_handler sh = sig_handle;
	signal_handler ret;
	
	ret = signal(SIGUSR1, sh);
	ret = signal(SIGINT, sh);
	ret = signal(SIGSTOP, sh);
	/*
	if(SIG_ERR == ret){
		printf("mount SIGSTOP hanlder funciton failuer.\n");
		return -1;
	}
	*/

	ret = signal(SIGKILL, sh);
	/*
	if(SIG_ERR == ret){
		printf("mount SIGKILL hanlder funciton failuer.\n");
		return -1;
	}
	*/
	
	
	int i,r;
	for(i = 0; i < 10000; ++i){
		if(1234 == i) {
			r = kill(getpid(), SIGUSR1);
		}
	}

	for(;;);
}

