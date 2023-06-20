#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /*
    int socket(int __domain, int __type, int __protocol)
    - domain:协议族
    - type:通信过程中的协议类型
    - protocol:具体协议,一般写0(流式协议默认使用TCP、报式协议使用UDP)
    */
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
    // char *server_ip = "192.168.42.130";
    char *server_ip = "127.0.0.1";
    inet_pton(AF_INET, server_ip, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(8090);
    int ret = bind(lfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3、设置监听
    ret = listen(lfd, 8);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // 4、设置阻塞等待连接
    struct sockaddr_in clientaddr;
    int addr_len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &addr_len);
    if (cfd == -1)
    {
        perror("accept");
        exit(0);
    }

    char client_ip[16] = {0};
    inet_ntop(clientaddr.sin_family, &clientaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    unsigned int client_port = ntohs(clientaddr.sin_port);
    printf("客户端IP: %s, 端口:%d\n", client_ip, client_port);

    // 5、开始通信
    char recvBuf[1024] = {0};
    while (1)
    {
        int len = read(cfd, recvBuf, sizeof(recvBuf));
        if (len == 0)
        {
            printf("client closed...\n");
            break;
        }
        else if (len > 0)
        {
            printf("server rev data : %s\n", recvBuf);
        }
        else if (len == -1)
        {
            perror("read");
        }
        char *sendBuf = "i am server";
        len = write(cfd, sendBuf, strlen(sendBuf));
        // printf("server send data len : %d\n", len);
    }

    close(cfd);
    close(lfd);

    return 0;
}
