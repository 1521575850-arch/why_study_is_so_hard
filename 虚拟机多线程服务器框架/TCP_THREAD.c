#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>

#define DEF_VAL 0
#define ERR_VAL -1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *client_handle(void *arg)
{
    int connfd = *(int *)arg;
    pthread_mutex_unlock(&mutex);
    int ret = 0;
    char buf[1024] = {0};
    while (1)
    {
        ret = read(connfd, buf, sizeof(buf));
        if (ret == 0)
        {
            printf("服务器断开连接\n");
            break;
        }
        else if (ret == ERR_VAL)
        {
            printf("通信异常\n");
            break;
        }
        printf("读到消息：%s", buf);
        write(connfd, buf, ret);
        memset(buf, 0, sizeof(buf));
    }
}
int main(int argc, const char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, DEF_VAL);
    if (sockfd == ERR_VAL)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in saddr, caddr;
    socklen_t caddr_len = sizeof(caddr);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(8888);

    int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == ERR_VAL)
    {
        perror("bind");
        exit(1);
    }
    ret = listen(sockfd, 10);
    if (ret == ERR_VAL)
    {
        perror("listen");
        exit(1);
    }
    int connfd = 0;
    pthread_t tid;
    while (1)
    {
        pthread_mutex_unlock(&mutex);
        connfd = accept(sockfd, (struct sockaddr *)&caddr, &caddr_len);
        if (connfd == ERR_VAL)
        {
            perror("accept");
            exit(1);
        }
        char caddr_data[20] = {0};
        printf("客户端%s：%d发起连接\n",
               inet_ntop(caddr.sin_family, &caddr.sin_addr, caddr_data, sizeof(caddr_data)), ntohs(caddr.sin_port));
        pthread_create(&tid, NULL, client_handle, &connfd);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}