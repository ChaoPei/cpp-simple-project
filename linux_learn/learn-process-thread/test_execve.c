#include <stdio.h>
#include <unistd.h>

int main(void) {
	
	// call run process
	if(execve("./run", NULL,  NULL) < 0)
		printf("execve failure !\n");

	return 0;
}
