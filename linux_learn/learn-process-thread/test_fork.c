#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	pid_t pid;
	
	// create child process
	pid = fork();

	if(-1 == pid) {
		printf("fork failure!\n");
		return -1;
	}
	else if(0 == pid) {	// child process
		printf("I am child: %d, my parent is: %d\n", getpid(), getppid());
	}
	else { // parent
		printf("I am parent: %d\n", getpid());
	}

	return 0;
}

