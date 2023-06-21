#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>

/*
    多进程实现高并发tcp服务器
        父线程阻塞等待连接客户端
        子线程处理通信

    利用信号量回收子线程资源

*/

// 回收资源
void recyleChild(int arg)
{

    while (1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1)
        {
            // 子进程已全部回收
            break;
        }
        else if (ret == 0)
        {
            // 还有子进程没回收
            break;
        }
        else if (ret > 0)
        {
            printf("子进程 %d 被回收\n", ret);
        }
    }
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recyleChild;
    sigaction(SIGCHLD, &act, NULL);

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

    while (1)
    {
        struct sockaddr_in clientaddr;
        int addr_len = sizeof(clientaddr);
        // 4、阻塞等待客户端连接
        int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &addr_len);
        if (cfd == -1)
        {
            // 如果是信号量打断阻塞等待
            if(errno == EINTR){
                continue;
            }
            perror("accept");
            exit(-1);
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            // 子线程
            char client_ip[16] = {0};
            inet_ntop(clientaddr.sin_family, &clientaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            unsigned short client_port = ntohs(clientaddr.sin_port);
            printf("客户端IP: %s, 端口:%d\n", client_ip, client_port);
            char recvBuf[1024];
            while (1)
            {
                int len = read(cfd, recvBuf, sizeof(recvBuf));
                if (len == -1)
                {
                    perror("read");
                    exit(-1);
                }
                else if (len == 0)
                {
                    printf("client closed ...\n");
                    break;
                }
                else if (len > 0)
                {
                    printf("server recv : %s\n", recvBuf);
                }

                write(cfd, recvBuf, strlen(recvBuf) + 1);
            }
            close(cfd);
            exit(0);
        }
    }
    close(lfd);

    return 0;
}
