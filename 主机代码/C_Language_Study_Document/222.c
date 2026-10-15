#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUF_SIZE 1024
#define ERR(VAL)     \
    do               \
    {                \
        perror(VAL); \
        exit(1);     \
    } while (0)

/* 设置非阻塞模式 */
static int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        ERR("socket");
    }

    /* 地址复用 */
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); /* 修正：16位端口用htons */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

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
        ERR("set_nonblocking listen");
    }

    int epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1)
    {
        ERR("epoll create");
    }

    struct epoll_event events[MAX_EVENTS], evt;
    evt.data.fd = sockfd;
    evt.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &evt) == -1)
    {
        ERR("epoll ctl");
    }

    printf("server wait....\n");

    while (1)
    {
        int num = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (num == -1)
        {
            if (errno == EINTR)
                continue;
            ERR("epoll wait");
        }

        for (int i = 0; i < num; i++)
        {
            /* 处理新连接 */
            if (events[i].data.fd == sockfd)
            {
                /* 循环accept，处理所有就绪连接 */
                while (1)
                {
                    socklen_t client_len = sizeof(client_addr);
                    int connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
                    if (connfd == -1)
                    {
                        if (errno == EINTR)
                            continue;
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        perror("accept");
                        break;
                    }

                    if (set_nonblocking(connfd) == -1)
                    {
                        perror("set_nonblocking conn");
                        close(connfd);
                        continue;
                    }

                    char client_ip[INET_ADDRSTRLEN] = {0};
                    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
                    printf("have client link\n");
                    printf("client %s:%d\n", client_ip, ntohs(client_addr.sin_port));

                    /* 用独立变量添加事件，不修改返回的events数组 */
                    struct epoll_event conn_evt;
                    conn_evt.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                    conn_evt.data.fd = connfd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &conn_evt) == -1)
                    {
                        perror("epoll ctl add conn");
                        close(connfd);
                        continue;
                    }
                }
            }
            /* 处理客户端连接事件 */
            else
            {
                int connfd = events[i].data.fd;
                int close_conn = 0;

                /* 错误/挂起事件 */
                if (events[i].events & (EPOLLERR | EPOLLHUP))
                {
                    close_conn = 1;
                }

                /* 可读事件 */
                if (!close_conn && (events[i].events & EPOLLIN))
                {
                    char buf[BUF_SIZE] = {0};
                    ssize_t ret = read(connfd, buf, sizeof(buf) - 1);

                    if (ret == -1)
                    {
                        /* 非阻塞正常情况：数据读完或被中断 */
                        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                        {
                            continue;
                        }
                        perror("read");
                        close_conn = 1;
                    }
                    else if (ret == 0)
                    {
                        printf("client close\n");
                        close_conn = 1;
                    }
                    else
                    {
                        printf("client push :%s\n", buf);
                        /* 简单回写，非阻塞下尽量发送 */
                        ssize_t wret = write(connfd, buf, ret);
                        if (wret == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
                        {
                            perror("write");
                            close_conn = 1;
                        }
                    }
                }

                /* 关闭连接，清理资源 */
                if (close_conn)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
                    close(connfd);
                }
            }
        }
    }

    close(sockfd);
    close(epfd);
    return 0;
}