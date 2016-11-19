#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8844
#define BACKLOG 3

// process client connection
void process_conn_server(int sc);

int main(int argc, char* argv[]) {

	int ss, sc;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;

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


// server process connection
void process_conn_server(int sc) {
	ssize_t size = 0;
	char buffer[1024];

	for(;;) {
		size = read(sc, buffer, 1024);
		if(size == 0) {
			return;
		}
		
		printf("Recived %d bytes...\n", size);

		// build response and send to client socket
		sprintf(buffer, "%d bytes altogether\n", size);
		write(sc, buffer, strlen(buffer)+1);
	}
}
	

