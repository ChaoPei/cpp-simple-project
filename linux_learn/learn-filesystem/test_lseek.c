#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void) {
	int i;
	int fd = -1;
	ssize_t size = -1;
	off_t offset = -1;
	char buf1[] = "0123456789";
	char buf2[] = "ABCDEFG";
	char filename[] = "hole.txt";

	int len = sizeof(buf1) / sizeof(buf1[0]);
	fd = open(filename, O_RDWR | O_CREAT, S_IRWXU);
	if(-1 == fd) {
		printf("open file failure !\n");
		return -1;
	}

	size = write(fd, buf1, len);
	if(size != len) {
		printf("write file failure !\n");
		return -1;
	}

	// lseek beyond file size range
	offset = lseek(fd, 32, SEEK_SET);
	if(-1 == offset) {
		printf("lseek set offset failure !\n");
		return -1;
	}
	
	len = sizeof(buf2) / sizeof(buf2[0]);
	size = write(fd, buf2, len);
	if(size != len) {
		printf("write file failure !\n");
		return -1;
	}

	close(fd);
	return 0;
}
		
