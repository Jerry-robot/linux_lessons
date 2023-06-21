#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

/*
    基于多线程实现高并发tcp服务器
        主线程阻塞接受客户端连接
        子线程处理接收数据

    增加：端口复用
*/

struct SockInfo
{
    int fd;
    struct sockaddr_in addr;
    pthread_t tid;
};

struct SockInfo sockinfos[128];

void *working(void *arg)
{
    struct SockInfo *pinfo = (struct SockInfo *)arg;
    char client_ip[16] = {0};
    inet_ntop(pinfo->addr.sin_family, &pinfo->addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    unsigned int client_port = ntohs(pinfo->addr.sin_port);
    printf("客户端IP: %s, 端口:%d\n", client_ip, client_port);
    char recvBuf[1024];
    while (1)
    {
        // int len = read(pinfo->fd, recvBuf, sizeof(recvBuf));
        int len = recv(pinfo->fd, recvBuf, sizeof(recvBuf), 0);
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
        for (int i = 0; i < len; ++i)
        {
            recvBuf[i] = toupper(recvBuf[i]);
        }
        // int ret = write(pinfo->fd, recvBuf, strlen(recvBuf));
        int ret = send(pinfo->fd, recvBuf, strlen(recvBuf), 0);
        if(ret == -1){
            perror("send");
            exit(-1);
        }
    }
    close(pinfo->fd);
    return 0;
}

int main(int argc, char *argv[])
{
    // 1、创建套接字
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 2、绑定IP与端口号
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    char *server_ip = "192.168.42.130";
    unsigned short int server_port = 8090;
    serveraddr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &serveraddr.sin_addr.s_addr);
    // 设置端口复用
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    int ret = bind(lfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    // 3、开启监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }
    // 初始化数据SockInfos
    int info_num = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < info_num; i++)
    {
        memset(&sockinfos[i], 0, sizeof(sockinfos[i]));
        // bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }
    while (1)
    {
        struct SockInfo *pingInfo;
        for (int i = 0; i < info_num; i++)
        {
            if (sockinfos[i].fd == -1)
            {
                pingInfo = &sockinfos[i];
                break;
            }
            if (i == info_num - 1)
            {
                sleep(1);
                i = -1;
            }
        }

        // struct sockaddr_in clientaddr;
        int addr_len = sizeof(pingInfo->addr);
        // 4、阻塞等待客户端连接
        pingInfo->fd = accept(lfd, (struct sockaddr *)&pingInfo->addr, &addr_len);
        if (pingInfo->fd == -1)
        {
            perror("accept");
            exit(-1);
        }

        int ret = pthread_create(&pingInfo->tid, NULL, &working, pingInfo);
        if (ret == -1)
        {
            perror("pthread_create");
            exit(-1);
        }
    }

    close(lfd);

    return 0;
}
