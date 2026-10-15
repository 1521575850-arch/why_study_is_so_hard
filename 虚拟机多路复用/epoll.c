#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8888       // 监听端口
#define MAX_EVENTS 1024 // 单次 epoll_wait 最多返回的事件数

int main()
{
    // 1. 创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(1);
    }
    // 2. bind：绑定本机所有网卡的 PORT 端口
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有网卡
    if (bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    // 3. listen：进入监听状态，允许排队 128 个连接
    if (listen(lfd, 128) == -1)
    {
        perror("listen");
        exit(1);
    }
    // 4. 创建 epoll 实例
    int epfd = epoll_create1(0);
    if (epfd == -1)
    {
        perror("epoll_create1");
        exit(1);
    }
    // 5. 把监听套接字 lfd 注册进 epoll（关心"有新连接来"这一事件）
    struct epoll_event ev, evs[MAX_EVENTS];
    ev.events = EPOLLIN; // 水平触发（默认），简单直观
    ev.data.fd = lfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev) == -1)
    {
        perror("epoll_ctl");
        exit(1);
    }
    printf("服务器已启动，监听端口 %d ...\n", PORT);
    // 6. 事件循环：阻塞等待事件，一次处理所有就绪的 fd
    while (1)
    {
        int n = epoll_wait(epfd, evs, MAX_EVENTS, -1);
        if (n == -1)
        {
            perror("epoll_wait");
            continue;
        }
        for (int i = 0; i < n; ++i)
        {
            // 情况一：监听套接字可读 → 有新客户端来连接
            if (evs[i].data.fd == lfd)
            {
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(lfd, (struct sockaddr *)&caddr, &len);
                if (cfd == -1)
                {
                    perror("accept");
                    continue;
                }
                // 把新连接也注册进 epoll，之后只等它的数据
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev) == -1)
                {
                    perror("epoll_ctl");
                    close(cfd);
                    continue;
                }
                printf("新客户端接入：%s:%d (fd=%d)\n",
                       inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), cfd);
            }
            // 情况二：某个客户端套接字可读 → 收到数据 / 断开
            else
            {
                int cfd = evs[i].data.fd;
                char buf[1024];
                int ret = read(cfd, buf, sizeof(buf) - 1); // 留一个位置给 '\0'

                if (ret <= 0) // 返回 0=客户端关闭，-1=出错
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL); // 先注销再关闭
                    close(cfd);
                    printf("客户端断开 (fd=%d)\n", cfd);
                }
                else
                {
                    buf[ret] = '\0';
                    printf("收到客户端 (fd=%d)：%s", cfd, buf);
                    write(cfd, buf, ret); // 回显给客户端
                }
            }
        }
    }
    close(lfd);
    close(epfd);
    return 0;
}