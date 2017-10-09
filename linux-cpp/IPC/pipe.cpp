/**
 * 进程间通信：管道
 */

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

const std::string msg = "hello, this is a pipe";
const int buffer_size = 1024;

int main() {

    int fd[2];

    int *write_fd = &fd[1];
    int *read_fd = &fd[0];
    // 创建管道
    int result = pipe(fd);
    if(-1 == result) {
        std::cout << "create pipe fail" << std::endl;
        return -1;
    }

    pid_t pid = fork();
    if(-1 == pid) {
        std::cout << "fork fail" << std::endl;
        return -1;
    }
    if(0 == pid) {
        // 子进程中写管道，关闭读端
        close(*read_fd);
        write(*write_fd, msg.c_str(), msg.size());

        return 0;
    } else {
        // 父进程中读管道，关闭写端
        close(*write_fd);
        char buffer[buffer_size];
        
        int bytes = read(*read_fd, buffer, buffer_size);
        cout << "read " << bytes << " data: " << buffer;
    }
    return 0;
}