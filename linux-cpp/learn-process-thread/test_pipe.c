#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	int result = -1;
	int fd[2], nbytes;
	pid_t pid;
	char string[] = "hello pipe";
	char readbuf[80];
	
	// first fd is used to read, second fd is used to write
	int *read_fd = &fd[0];
	int *write_fd = &fd[1];

	// create pipe
	result = pipe(fd);
	
	if(-1 == result){
		printf("create pipe failuer!\n");
		return -1;
	}

	// create child process
	pid = fork();
	if(-1 == pid) {
		printf("fork child process failuer!\n");
		return -1;
	}
	
	// read and write to pipe throung pipe's fd
	
	if(0 == pid){	// child process
		close(*read_fd);
		// write datas to pipe
		result = write(*write_fd, string, strlen(string));

		return 0;
	}
	else {			// parent process
		close(*write_fd);
		// read datas to pipe
		nbytes = read(*read_fd, readbuf, sizeof(readbuf)/sizeof(readbuf[0]));

		printf("receive %d datas: %s\n", nbytes, readbuf);
		return 0;
	}
}




