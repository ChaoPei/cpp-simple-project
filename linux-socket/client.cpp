/**
 * 客户端实现
 */

#include <iostream>
#include <cstdlib>
#include <sys/ctypes>
#include <sys/csocket>
#include <cunistd>
#include <netinet/cin>
#include <csignal>
#include <string>

#define PORT 9527

extern void sig_process(int singo);
extern void sig_pipe(int signo);

// 客户端套接字
static int s;

void sig_process_client(int signo) {
    std::cout << "Catch a exit signal" << std::endl;
    close(s);
    exit(0);
}

void process_conn_client(int sockfd);

int main(int argc, char* argv[]) {
    
    // 服务器地址结构
    struct sockaddr_in server_addr;

    int err;

    if(argc == 1) {
        std::cout << "PLS input server address\n";
    }

    /* 注册信号处理 */

    // 挂接SIGINT信号的处理函数为sig_process
    signal(SIGINT, sig_process);
    // 挂接SIGPIPE信号处理函数为sig_pipe
    signal(SIGPIPE, sig_pipe);

    // 建立一个流式socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0) {
        std::cout << "Create scoket fail\n";
        return -1;
    }

    // 填充服务器地址
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);        
    server_addr.sin_port = htons(PORT);

    // 将用户输入的字符串转为IP地址类型，并设置为服务器的地址
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));    // 这里s是空的，当连接上服务端之后，获得一个连接后的socket，然后可以进行读写

    process_conn_client(s);
    close(s);
    return 0;
}


// 使用read和write进行IO操作
// void process_conn_client(int sockfd){
// 	ssize_t size = 0;
// 	char buffer[1024];

// 	for(;;) {
// 		// 从标准输入(键盘)读取数据
// 		size = read(0, buffer, 1024);
// 		if(size > 0) {
// 			// send to server
// 			write(ss, buffer, size);
// 			// receive from server
// 			size = read(ss, buffer, 1024);
// 			// write to std output(screen)
// 			write(1, buffer, size);
// 		}
// 	}
// }


// 使用recv和send进行IO操作
void process_conn_client(int sockfd) {
    ssize_t size = 0;
    constexpr int BUFF_LEN = 1024;
    char buffer[BUFF_LEN];

    while(1) {
        size = read(0, buffer, BUFF_LEN);    // 从标准输入(键盘)读取数据
        if(size > 0) {
            // 发送给服务端
            send(sockfd, buffer, size, 0);
            size = recv(sockfd, buffer, BUFF_LEN, 0);
            write(1, buffer, size);     // 打印在标准输出(屏幕)上
        }
    }
}