/**
 * 服务端
 */

#include <iostream>
#include <string>

#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

#include "signal_process.hpp"

#define PORT 9527
#define BACKLOG 2   // 监听队列等待长度

extern void sig_process(int signo);
extern void sig_pipe(int signo);

void process_conn_server(int sockfd);

int main(int argc, char* argv[]) {
    
    // 服务端和客户端socket
    int ss, sc;

    // 服务端和客户端地址
    struct sockaddr_in server_addr, client_addr;

    int err;

    pid_t pid;

    /* 注册信号监听事件 */
    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_pipe);

    // 创建服务端socket
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0) {
        std::cout << "Create server scoket fail\n";
        return -1;
    }

    // 填充服务器地址
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);        
    server_addr.sin_port = htons(PORT);

    // 绑定地址到socket上
    err = bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(err < 0) {
        std::cout << "Bind socket fail\n";
        return -1;
    }

    // 监听
    err = listen(ss, BACKLOG);
    if(err < 0) {
        std::cout << "listen socket fail\n";
        return -1;
    }

    // 等待客户端连接
    while(1) {
        
        socklen_t addrlen = sizeof(struct sockaddr);
        sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(sc < 0) {
            continue;
        }

        pid = fork();
        if(pid == 0){
            // 子进程中处理连接
            close(ss);
            process_conn_server(sc);
        } else {
            close(sc);
        }
    }
}

// 使用read和write进行IO操作
// void process_conn_server(int sc) {
// 	ssize_t size = 0;
// 	char buffer[1024];

// 	for(;;) {
// 		size = read(sc, buffer, 1024);
// 		if(size == 0) {
// 			return;
// 		}
		
// 		printf("Recived %d bytes...\n", size);

// 		// build response and send to client socket
// 		sprintf(buffer, "%d bytes altogether\n", size);
// 		write(sc, buffer, strlen(buffer)+1);
// 	}
// }

// 使用send和recv进行IO操作
void process_conn_server(int sockfd) {
    ssize_t size = 0;
    constexpr int BUFF_LEN = 1024;
    char buffer[BUFF_LEN];
    for(;;) {
        size = recv(sockfd, buffer, BUFF_LEN, 0);
        if(size == 0) {
            return;
        }
        std::cout << "receive client msg: " << buffer << std::endl;

        std::string to_client = std::string(buffer) + " bytes altogether\n";
        send(sockfd, to_client.c_str(), to_client.size(), 0);
    }
}