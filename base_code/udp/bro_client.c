#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

/*
    广播服务端
*/

int main(int argc, char *argv[])
{
    // 1、创建套接字
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = ntohs(8090);
    saddr.sin_addr.s_addr = INADDR_ANY;
    // 2、绑定
    bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));

    //开始通信
    while (1)
    {
        char recvBuf[1024];
        int ret = recvfrom(fd, recvBuf, sizeof(recvBuf), 0, NULL, NULL);
        printf("recvBuf : %s", recvBuf);

    }
    close(fd);
    return 0;
}
