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
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = ntohs(8090);

    // 2、绑定ip和端口
    bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));


    //开始通信
    while (1)
    {
        char recvBuf[1024];
        struct sockaddr_in clientaddr;
        int len = sizeof(clientaddr);
        int ret = recvfrom(fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&clientaddr, &len);
        char client_ip[16];
        inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
        printf("client ip :%s, port :%d\n", client_ip, clientaddr.sin_port);
        if(ret == -1){
            perror("recvfrom");
            exit(-1);
        }
        printf("client say : %s", recvBuf);
        sendto(fd, recvBuf, strlen(recvBuf)+1, 0, (struct sockaddr*)&clientaddr, len);

    }
    close(fd);
    return 0;
}
