#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 8080

#define ERR(VAL)     \
    do               \
    {                \
        perror(VAL); \
        exit(1);     \
    } while (0)
// 将文件描述符设置为非阻塞
static int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        ERR("socket");
    }
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // 端口使用 htons 转换
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t client_len = sizeof(client_addr);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        ERR("bind");
    }
    if (listen(sockfd, 10) == -1)
    {
        ERR("listen");
    }
    if (set_nonblocking(sockfd) == -1)
    {
        ERR("fcntl socket");
    }
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1)
    {
        ERR("epoll_create1");
    }
    struct epoll_event fd[10], evt;
    // 将监听 socket 加入 epoll，等待新连接
    evt.data.fd = sockfd;
    evt.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &evt) == -1)
    {
        ERR("epoll_ctl");
    }
    printf("server wait....\n");
    while (1)
    {
        int num = epoll_wait(epfd, fd, 10, -1);
        if (num == -1)
        {
            ERR("epoll_wait");
        }
        for (int i = 0; i < num; i++)
        {
            // 监听 socket 有事件，说明有客户端连接
            if (fd[i].data.fd == sockfd)
            {
                char client_ip[INET_ADDRSTRLEN] = {0};
                memset(&client_addr, 0, sizeof(client_addr));
                client_len = sizeof(client_addr); // 每次 accept 前重置地址长度
                int connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
                if (connfd == -1)
                {
                    perror("accept");
                    continue;
                }
                if (set_nonblocking(connfd) == -1)
                {
                    close(connfd);
                    ERR("fcntl client");
                }
                printf("have client link\n");
                printf("client %s:%d\n", inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip, sizeof(client_ip)), ntohs(client_addr.sin_port)); // 将客户端 socket 加入 epoll，等待客户端发送数据
                evt.events = EPOLLIN;
                evt.data.fd = connfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &evt) == -1)
                {
                    ERR("epoll_ctl connfd");
                }
            }
            else if (fd[i].events & EPOLLIN)
            {
                char buf[1024] = {0};
                int connfd = fd[i].data.fd;
                ssize_t ret = read(connfd, buf, sizeof(buf));
                if (ret == -1)
                { // 出错时关闭客户端 socket，不要关闭监听 socket
                    epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
                    close(connfd);
                    ERR("read");
                }
                else if (ret == 0)
                { // 客户端关闭连接
                    printf("client close\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
                    close(connfd);
                    continue; // 继续处理本轮其他就绪事件
                }
                else
                { // 按实际读取长度打印，避免缓冲区满时越界读取
                    printf("client push: %.*s\n", (int)ret, buf);
                    write(connfd, buf, ret);
                }
            }
        }
    }
    close(sockfd);
    return 0;
}