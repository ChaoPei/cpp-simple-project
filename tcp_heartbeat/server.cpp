#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <netinet/in.h>
#include <string>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

#define BUFFER_SIZE 1024;

enum Type
{
    HEART,
    OTHER
};

struct PACKET_HEAD
{
    Type type;
    int length;
};

void *heart_handler(void *arg);

class Server
{
public:
    Server(int port);
    ~Server();
    void run();
    void _bind();
    void _listen(int queue_len = 20);
    void _accept();
    void _recv(int nums);
    friend void *heart_handler(void *arg);

private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int listen_fd;      // 监听连接请求
    int max_fd;         // server使用的文件描述符最大值
    fd_set master_set;  // 使用的文件描述符的集合
    fd_set working_set; // 存活的文件描述符的集合
    struct timeval timeout;
    map<int, pair<string, int>> mmap;
};

Server::Server(int port)
{
    // 建立 socket 监听端口
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        printf("Create socket failed!");
        exit(1);
    }
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

Server::~Server()
{
    // 释放使用的文件描述符
    for (int fd = 0; fd <= max_fd; ++fd)
    {
        if (FD_ISSET(fd, &master_set))
        {
            close(fd);
        }
    }
}

void Server::_bind()
{
    if (-1 == (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))))
    {
        printf("Server bind failed!");
        exit(1);
    }
    printf("Bind successfully\n");
}

void Server::_listen(int queue_len)
{
    if (-1 == listen(listen_fd, queue_len))
    {
        printf("Server listen failed!");
        exit(1);
    }
    printf("Listen successfully\n");
}

void Server::_accept()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int new_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (new_fd < 0)
    {
        printf("Server accept failed!");
        exit(1);
    }
    string ip(inet_ntoa(client_addr.sin_addr));
    printf("%s new connection was accepted.");
    mmap.insert(make_pair(new_fd, make_pair(ip, 0)));

    // 将新连接的 fd 加入 master_set
    FD_SET(new_fd, &master_set);
    max_fd = new_fd > max_fd ? new_fd : max_fd;
}

void Server::_recv(int nums)
{
    for (int fd = 0; fd <= max_fd; ++fd)
    {
        // 对于一个有效连接
        if (FD_ISSET(fd, &working_set))
        {
            bool close_conn = false;

            // 接收包的自定义头部并判断类型（是否为心跳包）
            PACKET_HEAD head;
            recv(fd, &head, sizeof(head), 0);
            if (head.type == HEART)
            {
                // 连接定时器清零
                mmap[fd].second = 0;
                printf("Recvived heartbeat from client\n");
            }
            else
            {
                // data process
            }

            // 需要关闭这个连接
            if (close_conn)
            {
                close(fd);
                FD_CLR(fd, &master_set);
                if (fd == max_fd)
                {
                    while (FD_ISSET(max_fd, &master_set) == false)
                    {
                        --max_fd;
                    }
                }
            }
        }
    }
}

void Server::run()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL, heart_handler, (void *)this);
    if (ret != 0)
    {
        printf("Can not create heartbeat checing thread.\n");
    }
    max_fd = listen_fd;
    FD_ZERO(&master_set);
    FD_SET(listen_fd, &master_set);
    while (1)
    {
        FD_ZERO(&working_set);
        memcpy(&working_set, &master_set, sizeof(master_set));
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
        int nums = select(max_fd + 1, &working_set, NULL, NULL, &timeout);
        if (nums < 0)
        {
            printf("select() error!");
        }
        // 没有连接发送数据过来
        if (nums == 0)
        {
            continue;
        }
        // 如果是有请求过来
        if (FD_ISSET(listen_fd, &working_set))
        {
            this->_accept();
        }
        else
        {
            this->_recv(nums);
        }
    }
}

// 不停的便利整个 master_set，间隔为 3 秒，
// 关闭超时的连接并移除他们的文件描述符，增加已有连接的定时器
void *heart_handler(void *arg)
{
    printf("The heartbeat checking thread started.\n");
    Server *srv = (Server *)arg;
    while (1)
    {
        map<int, pair<string, int>>::iterator it = srv->mmap.begin();
        for (; it != srv->mmap.end();)
        {
            if (it->second.second == 5)
            {
                printf("The client %s has be offline.\n", it->second.first);
                int fd = it->first;
                close(fd);
                FD_CLR(fd, &srv->master_set);
                if (fd == srv->max_fd)
                {
                    while ((FD_ISSET(srv->max_fd, &srv->master_set) == false))
                    {
                        srv->max_fd--;
                    }
                }
            }
            else if (it->second.second < 5 && it->second >= 0)
            {
                it->second.second += 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
        sleep(3);
    }
}

int main()
{
    Server server(15000);
    server._bind();
    server._listen();
    server.run();
    return 0;
}