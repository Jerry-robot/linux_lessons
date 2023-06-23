#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unlink("client.sock");


    // 1.创建监听套接字
    int fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(-1);
    }
    // 2.将监听套接字与本地套接字绑定
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "client.sock");
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }
    // 3.连接服务器
    struct sockaddr_un serveraddr;
    serveraddr.sun_family = AF_LOCAL;
    strcpy(serveraddr.sun_path, "server.sock");
    ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("connect");
        exit(-1);
    }
    // 4.通信
    int num = 0;
    while (1)
    {
        char sendBuf[1024];
        sprintf(sendBuf, "i am client %d\n", num++);
        send(fd, sendBuf, strlen(sendBuf) + 1, 0);
        int ret = recv(fd, sendBuf, sizeof(sendBuf), 0);
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
            printf("client recv : %s\n", sendBuf);
        }
        sleep(1);
    }
    close(fd);
    return 0;
}
