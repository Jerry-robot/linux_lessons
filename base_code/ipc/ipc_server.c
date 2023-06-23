#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unlink("server.sock");

    // 1.创建监听套接字
    int lfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 2.将监听套接字与本地套接字绑定
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server.sock");
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 100);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }
    // 4.等待并接受连接
    struct sockaddr_un clientaddr;
    int len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    if (cfd == -1)
    {
        perror("accept");
        exit(-1);
    }
    // 5.通信
    while (1)
    {
        char recvBuf[1024];
        int ret = recv(cfd, recvBuf, sizeof(recvBuf), 0);
        if (ret == -1)
        {
            perror("recv");
            exit(-1);
        }
        else if (ret == 0)
        {
            printf("client closed...\n");
            break;
        }
        else if (ret > 0)
        {
            printf("server recv : %s\n", recvBuf);
            send(cfd, recvBuf, strlen(recvBuf) + 1, 0);
        }
    }
    close(cfd);
    close(lfd);

    return 0;
}
