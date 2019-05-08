#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void){
	int fd = -1;
	char filename[] = "test_open.txt";
	fd = open(filename, O_RDONLY);
	if(-1 == fd) {
		printf("Open file %s failure!, fd: %d\n", filename, fd);
	}
	else {
		printf("Open file %s success!, fd: %d\n", filename, fd);
	}
	
	/*
	while(fd >= 0){
		fd = open(filename, O_RDONLY);
		printf("%d\n", fd);
		//int tmp = close(fd);
	}
	*/
	printf("error!\n");
	return 0;
}

