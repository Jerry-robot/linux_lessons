#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


/*
    广播客户端
*/


int main(int argc, char *argv[])
{
    // 1、创建套接字
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    // 2、设置广播属性
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    struct sockaddr_in bro_addr;
    bro_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.42.255", &bro_addr.sin_addr.s_addr);
    bro_addr.sin_port = ntohs(8090);

    // 开始通信
    int num = 0;
    while (1)
    {
        char sendBuf[1024];
        sprintf(sendBuf, "broadcast data : %d\n", num++);
        printf("sendBuf: %s", sendBuf);
        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr *)&bro_addr, sizeof(bro_addr));
        sleep(1);
    }
    close(fd);
    return 0;
}
