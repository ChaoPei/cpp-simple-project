#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8844

// process connection to server
void process_conn_client(int ss);

int main(int argc, char* argv[]) {
	int ss;
	struct sockaddr_in server_addr;
	int err;

	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0) {
		printf("Create server socket failure !\n");
		return -1;
	}

	// set server addr
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	// 将用户输入的服务器ip地址字符串转换为socket中ip地址
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	// connect to server
	connect(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	
	
	process_conn_client(ss);

	close(ss);
}


void process_conn_client(int ss){
	ssize_t size = 0;
	char buffer[1024];

	for(;;) {
		// 从标准输入(键盘)读取数据
		size = read(0, buffer, 1024);
		if(size > 0) {
			// send to server
			write(ss, buffer, size);
			// receive from server
			size = read(ss, buffer, 1024);
			// write to std output(screen)
			write(1, buffer, size);
		}
	}
}
	

