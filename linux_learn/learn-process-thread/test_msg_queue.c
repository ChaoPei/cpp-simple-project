// run as sudo root

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>

#define MSG_SIZE 10
#define MSG_TYPE 10

/**
 * show message queue's attributes
 * msq_id: message queue id
 * msq_info: message queue datastruct info
 *
 * read msg queue from kernel and storage in the buffer msqid_ds
 */
void show_msg_queue_attr(int msq_id, struct msqid_ds msq_info) {
	
	int ret = -1;
	sleep(1);
	// get message 
	ret = msgctl(msq_id, IPC_STAT, &msq_info);
	if(-1 == ret){
		printf("get message queue failure!\n");
		return;
	}

	printf("\n");
	printf("现在队列中最大字节数：%d\n", msq_info.msg_cbytes);
	printf("队列中消息数：%d\n", msq_info.msg_qnum);
	printf("队列中最大字节数：%d\n", msq_info.msg_qbytes);
	printf("最后发送消息的进程：%d\n", msq_info.msg_lspid);
	printf("最后接收消息的进程：%d\n", msq_info.msg_lrpid);
	printf("最后发送消息的时间：%d\n", ctime(&(msq_info.msg_stime)));
	printf("最后接收消息的时间：%d\n", ctime(&(msq_info.msg_stime)));
	printf("最后变化时间：%d\n", ctime(&(msq_info.msg_ctime)));
	printf("消息队列UID：%d\n", msq_info.msg_perm.uid);
	printf("消息队列GID：%d\n", msq_info.msg_perm.gid);
}



int main(void) {
	int ret = -1;
	int msq_flags, msq_id;
	key_t key;
	
	// message buff ds
	struct msgmbuf{
		int mtype;
		char mtext[MSG_SIZE];
	};

	struct msqid_ds msq_info;
	struct msgmbuf msg_mbuf;
	
	// send and receive flags
	int msg_sflags, msg_rflags;
	char *msgpath = ".";
	
	// create message queue key
	key = ftok(msgpath, 'b');
	if(key != -1) {
		printf("success create key\n");
	}
	else {
		printf("create key failure\n");
	}

	// create message queue
	msq_flags = IPC_CREAT | IPC_EXCL;
	msq_id = msgget(key, msq_flags|0x0666);
	if(-1 == msq_id) {
		printf("create message queue failure\n");
		return 0;
	}
	
	// show message queue info
	show_msg_queue_attr(msq_id, msq_info);

	// send message 
	char msg_content[] = "test message";
	int msg_size = sizeof(msg_content) / sizeof(msg_content[0]);
	msg_mbuf.mtype = MSG_TYPE;
	msg_sflags = IPC_NOWAIT;
	memcpy(msg_mbuf.mtext, msg_content, msg_size);
	ret = msgsnd(msq_id, &msg_mbuf, msg_size, msg_sflags);

	if(-1 == ret) {
		printf("send message failure\n");
	}
	
	// show msg queue again
	show_msg_queue_attr(msq_id, msq_info);
	
	// receive message 
	msg_rflags = IPC_NOWAIT | MSG_NOERROR;
	ret = msgrcv(msq_id, &msg_mbuf, MSG_SIZE, MSG_TYPE, msg_rflags);
	if(-1 == ret) {
		printf("receive message failure\n");
	}
	else {
		printf("receive message success, length: %d\n", ret);
	}

	show_msg_queue_attr(msq_id, msq_info);
	
	// set message queue attr
	msq_info.msg_perm.uid = 8;
	msq_info.msg_perm.gid = 8;
	msq_info.msg_qbytes = 12345;
	ret = msgctl(msq_id, IPC_SET, &msq_info);
	if(-1 == ret) {
		printf("set message queue failure\n");
		return 0;
	}
	
	show_msg_queue_attr(msq_id, msq_info);

	ret = msgctl(msq_id, IPC_RMID, NULL);
	if(-1 == ret) {
		printf("delete message queue failure\n");
		return 0;
	}

	return 0;
}


	
