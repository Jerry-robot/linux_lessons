#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


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

    // 加入到多播组
    struct ip_mreq op;
    inet_pton(AF_INET, "239.0.0.10", &op.imr_multiaddr.s_addr);
    op.imr_interface.s_addr = INADDR_ANY;
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op));

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
