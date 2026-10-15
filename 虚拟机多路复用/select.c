#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    int fd = open("/dev/input/mice", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    fd_set fds, tmp;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    FD_SET(0, &fds);

    int nfds = fd + 1;
    char buf[1024] = {0};
    while (1)
    {
        tmp = fds;
        int s_ret = select(fd + 1, &tmp, NULL, NULL, NULL);
        if (s_ret == -1)
        {
            perror("select");
            break;
        }
        for (int i = 0; i < nfds; i++)
        {
            if (FD_ISSET(i, &tmp))
            {
                if (i == 0)
                {
                    fgets(buf, sizeof(buf), stdin);
                    printf("buf:%s", buf);
                }
                else if (i == fd)
                {
                    int ret = read(fd, buf, sizeof(buf));
                    if (ret <= 0)
                    {
                        return 0;
                    }
                    printf("%d--%d--%d\n", buf[0], buf[1], buf[2]);
                }
                memset(buf, 0, sizeof(buf));
            }
        }
    }
    return 0;
}
