#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

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

    fd_set rfds, tmp;

    FD_ZERO(&rfds);
    FD_SET(lfd, &rfds);
    int nfd = lfd;
    while (1)
    {
        tmp = rfds;
        int ret = select(nfd + 1, &tmp, NULL, NULL, NULL);
        if (ret == -1)
        {
            perror("select");
            exit(-1);
        }
        else if (ret == 0)
        {
            continue;
        }
        else if (ret > 0)
        {
            if (FD_ISSET(lfd, &tmp))
            {
                struct sockaddr_in clientaddr;
                int len = sizeof(clientaddr);
                int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
                nfd = nfd > cfd ? nfd : cfd;
                FD_SET(cfd, &rfds);
            }
            for (int i = lfd + 1; i < nfd + 1; i++)
            {
                if (FD_ISSET(i, &tmp))
                {
                    char recvBuf[1024];
                    int read_ret = read(i, recvBuf, sizeof(recvBuf));
                    if (read_ret == -1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if (read_ret == 0)
                    {
                        printf("client close...\n");
                        close(i);
                        FD_CLR(i, &rfds);
                    }
                    else if (read_ret > 0)
                    {
                        printf("read data : %s\n", recvBuf);
                        write(i, recvBuf, strlen(recvBuf) + 1);
                    }
                }
            }
        }
    }

    close(lfd);
    FD_CLR(lfd, &rfds);

    return 0;
}
