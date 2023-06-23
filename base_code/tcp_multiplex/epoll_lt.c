#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

/*
    epoll 水平触发level_triggered
*/

int main(int argc, char *argv[])
{
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
    // inet_pton(AF_INET, server_ip, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
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

    // 创建epoll实例
    int epfd = epoll_create(100);
    // 将监听fd与epoll实例绑定
    struct epoll_event epev;
    epev.data.fd = lfd;
    epev.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);
    if (ret == -1)
    {
        perror("epoll_ctl");
        exit(-1);
    }

    struct epoll_event epevs[1024];
    // 开始通信
    while (1)
    {
        ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1)
        {
            perror("epoll_wait");
            exit(-1);
        }
        else
        {
            printf("ret num : %d\n", ret);
            for (int i = 0; i < ret; i++)
            {
                int curfd = epevs[i].data.fd;
                if (curfd == lfd)
                {
                    struct sockaddr_in clientaddr;
                    int len = sizeof(clientaddr);
                    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
                    epev.events = EPOLLIN;
                    epev.data.fd = cfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
                }
                else
                {
                    if (epevs[i].events & EPOLLOUT)
                    {
                        continue;
                    }
                    else
                    {
                        char recvBuf[5];
                        int read_ret = read(curfd, recvBuf, sizeof(recvBuf));
                        if (read_ret == -1)
                        {
                            perror("read");
                            exit(-1);
                        }
                        else if (read_ret == 0)
                        {
                            printf("client close...\n");
                            epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                            close(curfd);
                        }
                        else if (read_ret > 0)
                        {
                            printf("read data : %s\n", recvBuf);
                            printf("read len :%d, write len : %ld\n", read_ret, strlen(recvBuf) + 1);
                            write(curfd, recvBuf, strlen(recvBuf) + 1);
                        }
                    }
                }
            }
        }
    }
    epoll_ctl(epfd, EPOLL_CTL_DEL, lfd, NULL);
    close(epfd);
    close(lfd);
    return 0;
}
