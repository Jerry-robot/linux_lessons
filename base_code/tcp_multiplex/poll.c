#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

int main(int argc, char *argv[])
{
    struct pollfd pollfds[1024];
    for (int i = 0; i < 1024; i++)
    {
        pollfds[i].fd = -1;
        pollfds[i].events = POLLIN | POLLOUT;
    }

    // 1、创建套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }
    // 2、ip、port和fd绑定
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    char *server_ip = "192.168.42.130";
    // char *server_ip = "127.0.0.1";
    inet_pton(AF_INET, server_ip, &serveraddr.sin_addr.s_addr);
    // serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(8090);
    int ret = bind(lfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 8);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }
    pollfds[0].fd = lfd;
    nfds_t nfds = 0;

    while (1)
    {
        // int ret = select(nfd + 1, &tmp, NULL, NULL, NULL);
        ret = poll(pollfds, nfds + 1, -1);
        if (ret == -1)
        {
            perror("poll");
            exit(-1);
        }
        else if (ret == 0)
        {
            continue;
        }
        else if (ret > 0)
        {
            if (pollfds[0].revents & POLLIN)
            {
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
                for (int i = 1; i < 1024; i++)
                {
                    if (pollfds[i].fd == -1)
                    {
                        pollfds[i].fd = cfd;
                        nfds = nfds > i ? nfds : i;
                        break;
                    }
                }
            }
            for (int i = 1; i <= nfds; i++)
            {
                if (pollfds[i].revents & POLLIN)
                {
                    char recvBuf[1024];
                    int read_ret = read(pollfds[i].fd, recvBuf, sizeof(recvBuf));
                    if (read_ret == -1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if (read_ret == 0)
                    {
                        printf("client close...\n");
                        close(pollfds[i].fd);
                        pollfds[i].fd = -1;
                    }
                    else if (read_ret > 0)
                    {
                        printf("read data : %s\n", recvBuf);
                        write(pollfds[i].fd, recvBuf, strlen(recvBuf) + 1);
                    }
                }
            }
        }
    }

    close(lfd);
    pollfds[0].fd = -1;
    return 0;
}
