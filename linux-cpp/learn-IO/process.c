#include "process.h"

void process_conn_client(int s){
	ssize_t size = 0;
	char buffer[30];
	struct msghdr msg;
	
	// 申请3个iovec向量
	struct iovec *v = (struct iovec*)malloc(3*sizeof(struct iovec));
	if(!v) {
		printf("malloc for iovec is failure\n");
		return;
	}

	vc = v;
	
	// 初始化msghdr
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iov = v;
	msg.msg_iovlen = 30;
	msg.msg_flags = 0;
	
	// iovec大小设置为10个字节
	v[0].iov_len = 10;
	v[1].iov_len = 10;
	v[2].iov_len = 10;
	
	// 绑定向量指向的缓冲区地址
	// v保存的是三个向量, 每个向量实际的缓冲区在buffer中
	v[0].iov_base = buffer;
	v[1].iov_base = buffer+10;
	v[2].iov_base = buffer+20;

	int i = 0;
	for(;;) {
		// 将标准输入中读取到的数据放到缓冲区中
		size = read(0, v[0].iov_base, 10);
		if(size > 0) {
			v[0].iov_len = size;
			// 发送消息
			sendmsg(s, &msg, 0);
			v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;

			// 接收消息(阻塞)
			size = recvmsg(s, &msg, 0);
			for(i = 0; i < 3; ++i) {
				// 写入到标准输出
				if(v[i].iov_len > 0) {
					write(1, v[i].iov_base, v[i].iov_len);
				}
			}
		}
	}
}



void process_conn_server(int s){
	ssize_t size = 0;
	char buffer[30];
	struct msghdr msg;
	
	// 申请3个iovec向量
	struct iovec *v = (struct iovec*)malloc(3*sizeof(struct iovec));
	if(!v) {
		printf("malloc for iovec is failure\n");
		return;
	}

	vc = v;
	
	// 初始化msghdr
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iov = v;
	msg.msg_iovlen = 30;
	msg.msg_flags = 0;
	
	// iovec大小设置为10个字节
	v[0].iov_len = 10;
	v[1].iov_len = 10;
	v[2].iov_len = 10;
	
	// 绑定向量指向的缓冲区地址
	// v保存的是三个向量, 每个向量实际的缓冲区在buffer中
	v[0].iov_base = buffer;
	v[1].iov_base = buffer+10;
	v[2].iov_base = buffer+20;

	int i = 0;
	for(;;) {
		// 从套接字中接收消息
		size = recvmsg(s, &msg, 0);
		if(size == 0) {
			return;
		}

		// 构建相应数据
		sprintf(v[0].iov_base, "%d", size);
		sprintf(v[1].iov_base, "bytes alt");
		sprintf(v[2].iov_base, "ogther\n");

		v[0].iov_len = strlen(v[0].iov_base);
		v[1].iov_len = strlen(v[1].iov_base);
		v[2].iov_len = strlen(v[2].iov_base);
	
		// 发送消息
		sendmsg(s, &msg, 0);
	}
}



void sig_process(int signo) {
	printf("Catch a exit signal\n");
	free(vc);
	free(vs);
	_exit(0);
}


void sig_pipe(int signo) {
	printf("Catch a pipe signal\n");
	free(vc);
	free(vs);
	_exit(0);
}


