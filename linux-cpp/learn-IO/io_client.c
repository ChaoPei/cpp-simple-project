#include <unistd.h>
#include <netinet/in.h>

#include "process.h"


// socket port
#define PORT 8844


int main(int argc, char* argv[]) {

    int s;
	
	// 服务器socket
	struct sockaddr_in server_addr;
	// 错误值
	int err;
	
	// 用户需要输入server地址
	if(argc <= 1) {
		printf("PLS input server ip addr\n");
		return 0;
	}
	
	
	// 注册信号处理函数
	signal(SIGINT, sig_process);
	signal(SIGPIPE, sig_pipe);


	// 创建一个流式的套接字用作客户端socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0) {
		printf("Create stream socket failure !\n");
		return -1;
	}


	// set server addr
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 暂时设置为本地的任意地址(所有地址)(可以接受本地任意网卡地址指定端口的数据, 而不仅仅是某一个本地IP的数据)
	server_addr.sin_port = htons(PORT);
	
	// 将用户输入的服务器ip地址字符串转换为socket中ip地址
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	// connect to server
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	
	process_conn_client(s);

	close(s);
}

