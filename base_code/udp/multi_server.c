#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


/*
    组播(多播)服务端
*/


int main(int argc, char *argv[])
{
    // 1、创建套接字
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    // 2、设置广播属性
    struct in_addr op;
    inet_pton(AF_INET, "239.0.0.10", &op.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &op, sizeof(op));

    struct sockaddr_in multi_addr;
    multi_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "239.0.0.10", &multi_addr.sin_addr.s_addr);
    multi_addr.sin_port = ntohs(8090);

    // 开始通信
    int num = 0;
    while (1)
    {
        char sendBuf[1024];
        sprintf(sendBuf, "multi broadcast data : %d\n", num++);
        printf("sendBuf: %s", sendBuf);
        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr *)&multi_addr, sizeof(multi_addr));
        sleep(1);
    }
    close(fd);
    return 0;
}
