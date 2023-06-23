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
    char *server_ip = "192.168.42.130";
    inet_pton(AF_INET, server_ip, &saddr.sin_addr.s_addr);

    //开始通信
    int num = 0;
    while (1)
    {
        char recvBuf[1024];
        sprintf(recvBuf, "num : %d\n", num++);
        sendto(fd, recvBuf, strlen(recvBuf)+1, 0, (struct sockaddr*)&saddr, sizeof(saddr));


        int ret = recvfrom(fd, recvBuf, sizeof(recvBuf), 0, NULL, NULL);
        if(ret == -1){
            perror("recvfrom");
            exit(-1);
        }
        printf("server say : %s", recvBuf);
        sleep(1);

    }
    close(fd);
    return 0;
}
