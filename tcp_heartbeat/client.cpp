#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 1024

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

void *send_heart(void *arg);

class Client
{
public:
    Client(string ip, int port);
    ~Client();
    void _connect();
    void run();
    friend void *send_heart(void *arg);

private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int fd;
};

Client::Client(string ip, int port)
{
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) == 0)
    {
        printf("Server IP Address Error!");
        exit(1);
    }
    server_addr.sin_port = htons(port);
    server_addr_len = sizeof(server_addr);
    // create socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
}

Client::~Client()
{
    close(fd);
}

void Client::_connect()
{
    printf("connecting......\n");
    if (connect(fd, (struct sockaddr *)&server_addr, server_addr_len) < 0)
    {
        printf("Can not _connect to Server IP!");
        exit(1);
    }
    printf("_connect to Server successfully.");
}

void Client::run()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL, send_heart, (void *)this);
    if (ret != 0)
    {
        printf("Can not create thread!");
        exit(1);
    }
}

void *send_heart(void *arg)
{
    printf("The heartbeat sending thread started.\n");
    Client *c = (Client *)arg;
    int count = 0; // 测试
    while (1)
    {
        PACKET_HEAD head;
        head.type = HEART;
        head.length = 0;
        send(c->fd, &head, sizeof(head), 0);
        sleep(3); // 定时3秒

        ++count; // 测试：发送15次心跳包就停止发送
        if (count > 15)
        {
            break;
        }
    }
}

int main()
{
    Client client("127.0.0.1", 15000);
    client._connect();
    client.run();
    while (1)
    {
        string msg;
        getline(cin, msg);
        if (msg == "exit")
            break;
        printf("msg\n");
    }
    return 0;
}
