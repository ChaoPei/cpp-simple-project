#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define IPADDRESS "127.0.0.1"
#define PORT 9527
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100

// 创建并绑定 socket
static int socket_bind(const char* ip, int port);

// 创建并注册 epoll 监听
static void do_epoll(int listenfd);

// epoll 事件处理
static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);

// socket 连接处理
static void handle_accept(int epollfd, int listenfd);

// socket 读取
static void do_read(int epollfd, int fd, char *buf);

// socket 写入
static void do_write(int epollfd, int fd, char *buf);

// epoll 添加事件
static void add_event(int epollfd, int fd, int state);

// epoll 修改事件
static void modify_event(int epollfd, int fd, int state);

// epoll 删除事件
static void delete_event(int epollfd, int fd, int state);

int main(int argc, char *argv[])
{
    // 要监听的 socket 描述符
    int listenfd;
    // 创建 socket 描述符并绑定 ip 和 port
    listenfd = socket_bind(IPADDRESS, PORT);
    // 开始监听，LISTENQ 是可以接收的最多排队等待的连接数目
    listen(listenfd, LISTENQ);
    // 使用 epoll 管理 IO 请求（socket 连接请求）
    do_epoll(listenfd);
    return 0;
}

static int socket_bind(const char *ip, int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    // 创建一个空的 socket 描述符
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 如果创建失败：一般是由于系统描述符不够用了
    if (listenfd == -1)
    {
        perror("socket error");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    // 将要监听的地址和端口与 socket 描述符绑定
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error");
        exit(1);
    };

    return listenfd;
}

static void do_epoll(int listenfd)
{
    // epoll 句柄描述符
    int epollfd;
    // 存储要监听得到的事件
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf, 0, MAXSIZE);
    // 创建 epoll 句柄
    epollfd = epoll_create(FDSIZE);
    // 添加监听 socket 描述符事件，状态为 EPOLLIN，即数据到来
    add_event(epollfd, listenfd, EPOLLIN);
    for(;;)
    {
        // 等待事件产生，设置能够监听得到最多 EPOLLEVENTS 个事件，timeout 为 -1，即不会超时，
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd, buf);
    }
    close(epollfd);
}

static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf)
{
    int i, fd;
    // 对所有监听得到的事件进行遍历
    for (int i = 0; i < num; i++)
    {
        // 获取事件的对应监听的目标描述符
        fd = events[i].data.fd;
        // 如果事件对应的描述符是我们要监听的描述符，并且其状态是我们需要的（连接请求到来）
        if ((fd == listenfd) && (events[i].events & EPOLLIN))
        {
            // 处理连接请求
            handle_accept(epollfd, listenfd);
        }
        // 如果是其他描述符，并且是数据读取就绪（本例中用于监听 client 描述符的读写）
        else if (events[i].events & EPOLLIN)
        {
            // 数据读取
            do_read(epollfd, fd, buf);
        }
        // 如果是数据写入就绪
        else if (events[i].events & EPOLLOUT)
        {
            do_write(epollfd, fd, buf);
        }
    }
}

static void handle_accept(int epollfd, int listenfd)
{
    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen;
    clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
    if (clientfd == -1)
    {
        perror("accept error:");
    }
    else
    {
        printf("accept a new client: %s:%d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
        add_event(epollfd, clientfd, EPOLLIN);
    }
}

static void do_read(int epollfd, int fd, char *buf)
{
    int nread;
    // 读取数据
    nread = read(fd, buf, MAXSIZE);
    if (nread == -1)
    {
        perror("read error");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else if (nread == 0)
    {
        fprintf(stderr, "client close.\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else
    {
        printf("read message: %s", buf);
        // 从描述符中读取数据完毕，则该描述符变为可写状态
        modify_event(epollfd, fd, EPOLLOUT);
    }
}

static void do_write(int epollfd, int fd, char *buf)
{
    int nwrite;
    // 写入数据
    nwrite = write(fd, buf, strlen(buf));
    if (nwrite == -1)
    {
        perror("write error");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    }
    else
    {
        // 写入数据到描述符完毕，则该描述符变为数据就绪事件，等待读取
        modify_event(epollfd, fd, EPOLLIN);
    }
    memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state)
{
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

static void modify_event(int epollfd, int fd, int state)
{
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);

}

static void delete_event(int epollfd, int fd, int state)
{
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
}
