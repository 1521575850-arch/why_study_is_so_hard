#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#define PORT 8088
#define MAX_EVENTS 1024
#define BUF_SIZE 1024

typedef struct
{
    int fd;
    char *outbuf;
    size_t outlen;
    size_t outsent;
} conn_t;

/* 设置文件描述符为非阻塞模式 */
static int set_nonblock(int fd)
{
    int fl = fcntl(fd, F_GETFL, 0);
    if (fl < 0)
    {
        return -1;
    }
    return fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

/* 创建连接结构体 */
static conn_t *conn_new(int fd)
{
    conn_t *c = malloc(sizeof(*c));
    if (c == NULL)
    {
        return NULL;
    }
    c->fd = fd;
    c->outbuf = NULL;
    c->outlen = 0;
    c->outsent = 0;
    return c;
}

/* 释放连接结构体 */
static void conn_free(conn_t *c)
{
    if (c == NULL)
    {
        return;
    }
    free(c->outbuf);
    free(c);
}

/* 追加数据到发送缓冲区 */
static int conn_append(conn_t *c, const char *data, size_t len)
{
    /* 如果有已发送但未清理的数据，先前移 */
    if (c->outsent > 0)
    {
        memmove(c->outbuf, c->outbuf + c->outsent, c->outlen - c->outsent);
        c->outlen -= c->outsent;
        c->outsent = 0;
    }
    /* 扩容 */
    char *p = realloc(c->outbuf, c->outlen + len);
    if (p == NULL)
    {
        return -1;
    }
    c->outbuf = p;
    memcpy(c->outbuf + c->outlen, data, len);
    c->outlen += len;
    return 0;
}

/* 冲刷发送缓冲区 */
static int conn_flush(conn_t *c)
{
    while (c->outsent < c->outlen)
    {
        ssize_t wn = send(c->fd, c->outbuf + c->outsent, c->outlen - c->outsent, 0);
        if (wn > 0)
        {
            c->outsent += wn;
            continue;
        }
        if (wn < 0 && errno == EINTR)
        {
            continue;
        }
        if (wn < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            return -1; /* 发送缓冲区满，稍后重试 */
        }
        return -1; /* 真正的错误 */
    }
    /* 全部发送完成，释放缓冲区 */
    free(c->outbuf);
    c->outbuf = NULL;
    c->outlen = 0;
    c->outsent = 0;
    return 0;
}

int main()
{
    int listenfd;
    int epfd;
    int opt;
    int n;
    int i;
    struct sockaddr_in addr;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];

    /* 忽略 SIGPIPE，防止写关闭的 socket 导致进程退出 */
    signal(SIGPIPE, SIG_IGN);

    /* 创建监听 socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket");
        return -1;
    }

    /* 设置地址复用 */
    opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* 绑定地址和端口 */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(listenfd);
        return 1;
    }

    /* 开始监听 */
    if (listen(listenfd, 128) < 0)
    {
        perror("listen");
        close(listenfd);
        return -1;
    }

    /* 设置监听 socket 为非阻塞 */
    if (set_nonblock(listenfd) < 0)
    {
        perror("set_nonblock listenfd");
        close(listenfd);
        return 1;
    }

    /* 创建 epoll 实例 */
    epfd = epoll_create1(0);
    if (epfd < 0)
    {
        perror("epoll_create1");
        close(listenfd);
        return 1;
    }

    /* 将监听 socket 加入 epoll */
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0)
    {
        perror("epoll_ctl ADD listenfd");
        close(epfd);
        close(listenfd);
        return 1;
    }

    printf("listen on %d\n", PORT);
    fflush(stdout);

    /* 事件循环 */
    while (1)
    {
        n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("epoll_wait");
            break;
        }

        for (i = 0; i < n; i++)
        {
            uint32_t rev = events[i].events;

            /* 处理新连接 */
            if (events[i].data.fd == listenfd)
            {
                while (1)
                {
                    int connfd = accept(listenfd, NULL, NULL);
                    if (connfd < 0)
                    {
                        if (errno == EINTR)
                        {
                            continue;
                        }
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            break; /* 没有更多连接了 */
                        }
                        perror("accept");
                        break;
                    }

                    /* 设置新连接为非阻塞 */
                    if (set_nonblock(connfd) < 0)
                    {
                        perror("set_nonblock connfd");
                        close(connfd);
                        continue;
                    }

                    /* 创建连接结构体 */
                    conn_t *c = conn_new(connfd);
                    if (c == NULL)
                    {
                        perror("conn_new");
                        close(connfd);
                        continue;
                    }

                    /* 将新连接加入 epoll，监听读事件 */
                    struct epoll_event e;
                    memset(&e, 0, sizeof(e));
                    e.events = EPOLLIN;
                    e.data.ptr = c;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &e) < 0)
                    {
                        perror("epoll_ctl ADD connfd");
                        conn_free(c);
                        close(connfd);
                        continue;
                    }
                }
                continue; /* 处理完监听事件，继续下一个事件 */
            }

            /* 处理普通连接的事件 */
            conn_t *c = events[i].data.ptr;
            int close_conn = 0;

            /* 错误或挂起事件 */
            if (rev & (EPOLLERR | EPOLLHUP))
            {
                close_conn = 1;
            }

            /* 可写事件：发送缓冲区数据 */
            if (!close_conn && (rev & EPOLLOUT))
            {
                int r = conn_flush(c);
                if (r == 0)
                {
                    /* 全部发送完成，取消监听写事件 */
                    struct epoll_event e;
                    memset(&e, 0, sizeof(e));
                    e.events = EPOLLIN;
                    e.data.ptr = c;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &e);
                }
                else if (r < 0)
                {
                    close_conn = 1;
                }
            }

            /* 可读事件：接收数据并回显 */
            if (!close_conn && (rev & EPOLLIN))
            {
                char buf[BUF_SIZE];
                while (1)
                {
                    ssize_t rn = recv(c->fd, buf, sizeof(buf), 0);
                    if (rn > 0)
                    {
                        /* 将收到的数据追加到发送缓冲区 */
                        if (conn_append(c, buf, rn) < 0)
                        {
                            close_conn = 1;
                            break;
                        }
                        /* 尝试立即发送 */
                        int r = conn_flush(c);
                        if (r == 0)
                        {
                            continue; /* 全部发完，继续读 */
                        }
                        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
                        {
                            close_conn = 1;
                            break;
                        }
                        /* 发送缓冲区满，注册写事件，等待可写时再发 */
                        struct epoll_event e;
                        memset(&e, 0, sizeof(e));
                        e.events = EPOLLIN | EPOLLOUT;
                        e.data.ptr = c;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &e);
                        break;
                    }
                    if (rn == 0)
                    {
                        /* 对端关闭连接 */
                        close_conn = 1;
                        break;
                    }
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        break; /* 数据已读完 */
                    }
                    if (errno == EINTR)
                    {
                        continue; /* 被信号中断，重试 */
                    }
                    perror("recv");
                    close_conn = 1;
                    break;
                }
            }

            /* 关闭连接 */
            if (close_conn)
            {
                epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
                close(c->fd);
                conn_free(c);
            }
        }
    }

    close(listenfd);
    close(epfd);
    return 0;
}
