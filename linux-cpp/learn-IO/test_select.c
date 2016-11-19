#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	// 读文件描述符集合
	fd_set rd;
	// select中timeout类型
	struct timeval tv;
	int err;

	FD_ZERO(&rd);
	// 向rd中加入标准输入(0),监视标准输入是否有可读数据
	FD_SET(0, &rd);

	// 设置等待超时为5s
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	// 因为rd中只有一个文件描述符0, 所以nfds设置为1
	err = select(1, &rd, NULL, NULL, &tv);

	if(err == -1) 
		perror("select()");
	else if(err)
		printf("Data is available now.\n");
	else
		printf("No data within five second.\n");

	return 0;
}




