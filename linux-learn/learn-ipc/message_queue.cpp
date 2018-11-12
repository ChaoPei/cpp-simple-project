/**
 * 进程间通信：消息队列
 */

// msg：消息
// msq：消息队列

#include <iostream>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/ipc.h>
#include <unistd.h>

const std::string msq_path = "/home/peic/ipc/msg/";
std::string test_msg = "test msg";

static void show_msq_attr(int msq_id, struct msqid_ds msq_info);

int main() {
    int ret = -1;
    
    // 消息的结构
    struct msgmbuf {
        int mtype;
        char mtext[10];
    };

    struct msqid_ds msq_info;
    struct msgmbuf msg_buf;

    // 获取一个队列的key
    key_t key = ftok(msq_path.c_str(), 'b');
    if(key == -1) {
        std::cout << "ftok fail" << std::endl;
        return -1;
    } 

    //  创建或者访问一个队列(通过key)
    std::cout << "key: " << key << std::endl;
    int msq_id = msgget(key, IPC_CREAT + 0x06666);
    if(-1 == msq_id) {
        std::cout << "msgget fail" << std::endl;
        return -1;
    }

    // 打印队列属性test
    show_msq_attr(msq_id, msq_info);

    // 发送消息
    std::copy(test_msg.begin(), test_msg.end(), msg_buf.mtext);
    ret = msgsnd(msq_id, &msg_buf, test_msg.size(), IPC_NOWAIT);
    if(-1 == ret) {
        std::cout << "msgsnd fail" << std::endl;
    }
    show_msq_attr(msq_id, msq_info);

    // 接收消息
    ret = msgrcv(msq_id, &msg_buf, 10, 10, IPC_NOWAIT|MSG_NOERROR);
    if(-1 == ret) {
        std::cout << "msgrcv fail" << std::endl;
    }
    show_msq_attr(msq_id, msq_info);

    // 设置消息队列的属性
    msq_info.msg_perm.uid = 8;
    msq_info.msg_perm.gid = 8;
    msq_info.msg_qbytes = 12345;
    ret = msgctl(msq_id, IPC_SET, &msq_info);
    if(-1 == ret) {
        std::cout << "msgctl set fail" << std::endl;
    }
    show_msq_attr(msq_id, msq_info);

    // 删除消息队列
    ret = msgctl(msq_id, IPC_RMID, NULL);
    if(-1 == ret) {
        std::cout << "msgctl remove fail" << std::endl;
        return -1;
    }
    return 0;
}



/**
 * 根据输入的消息ID，将消息队列中的字节数，消息数等属性打印出来
 */
void show_msq_attr(int msq_id, struct msqid_ds msq_info) {
    int ret = -1;
    sleep(-1);

    // 获取消息队列的结构，存在msg_info中
    ret = msgctl(msq_id, IPC_STAT, &msq_info);
    if(-1 == ret) {
        std::cout << "msgctl fail" << std::endl;
        return;
    }

    std::cout << "现在队列中的字节数: " << msq_info.msg_cbytes << std::endl;
    std::cout << "现在队列中的消息数: " << msq_info.msg_qnum << std::endl;
    std::cout << "队列中的最大字节数: " << msq_info.msg_qbytes << std::endl;
    std::cout << "最后发送消息的进程: " << msq_info.msg_lspid << std::endl;
    std::cout << "最后接收消息的进程: " << msq_info.msg_lrpid << std::endl;
    std::cout << "最后发送消息的时间: " << ctime(&(msq_info.msg_stime)) << std::endl;
    std::cout << "最后接收消息的时间: " << ctime(&(msq_info.msg_rtime)) << std::endl;
    std::cout << "最后变化的时间: " << ctime(&(msq_info.msg_ctime)) << std::endl;
    std::cout << "UID: " << msq_info.msg_perm.uid << std::endl;
    std::cout << "GID: " << msq_info.msg_perm.gid << std::endl;
}