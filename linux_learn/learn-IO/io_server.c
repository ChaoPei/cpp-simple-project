
#include <unistd.h>
#include <netinet/in.h>

#include "process.h"


#define PORT 8844
#define BACKLOG 3


int main(int argc, char* argv[]) {

	int ss, sc;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;
	
	signal(SIGINT, sig_process);
	signal(SIGPIPE, sig_pipe);


	// create server socket
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0) {
		printf("Create server socket failure !\n");
		return -1;
	}

	// set server addr
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// localhost
	server_addr.sin_port = htons(PORT);

	// bind socket
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0) {
		printf("Bind socket error!\n");
		return -1;
	}

	// set listen queue
	err = listen(ss, BACKLOG);
	if(err < 0){
		printf("Listen error!\n");
		return -1;
	}
	

	// main loop for accept client application
	for(;;) {
		socklen_t addrlen = sizeof(struct sockaddr);
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);

		if(sc < 0) 
			continue;
		
		// 每收到一个请求，开启一个子进程来处理请求
		pid = fork();
		if(pid == 0) {		//child process
			close(ss);
			process_conn_server(sc);
		}
		else {
			close(sc);		// parent process
		}
	}
}



