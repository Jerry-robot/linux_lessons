#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        exit(0);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    // char *server_ip = "192.168.42.130";
    char *server_ip = "127.0.0.1";
    int server_port = 8090;
    inet_pton(AF_INET, server_ip, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(server_port);
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (ret == -1)
    {
        perror("connect");
        exit(0);
    }
    char recvBuf[1024] = {0};
    while (1)
    {
        char *sendBuf = "i am client";
        int len = write(fd, sendBuf, strlen(sendBuf));
        // printf("client send data len : %d\n", len);
        len = read(fd, recvBuf, sizeof(recvBuf));
        if (len > 0)
        {
            printf("client rcv data : %s\n", recvBuf);
        }else if(len == 0){
            printf("server closed...");
        }else if(len == -1){
            perror("read");
            break;
        }
        sleep(1);
    }

    close(fd);

    return 0;
}
