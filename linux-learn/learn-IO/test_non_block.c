#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>



// socket port
#define PORT 8844


#define BACKLOG 4

int main(int argc, char* argv[]) {

    int ss = -1, sc = -1;
	struct sockaddr_in local, client;
	int err;
	int len;
	
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0) {
		printf("Create stream socket failure !\n");
		return -1;
	}

	// 设置为非阻塞方式
	fcntl(ss, F_SETFL, O_NONBLOCK);


	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr = htonl(-1);
	local.sin_port = htons(PORT);
	
	listen(ss, BACKLOG);
	
	for(;;) {

		// 非阻塞, accept()立即返回
		// 同步, 无法继续执行, 轮询
		while(sc < 0) ｛
			sc = accept(ss, (struct sockaddr*) &client, &len);
	}


	......
}

