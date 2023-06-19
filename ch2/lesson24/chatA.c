#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // 1、判断管道1是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1)
    {
        printf("FIFO1管道不存在,创建管道1\n");
        ret = mkfifo("fifo1", 0664);
        if (ret == -1)
        {
            perror("pipe");
            exit(0);
        }
    }
    ret = access("fifo2", F_OK);

    if (ret == -1)
    {
        printf("FIFO2管道不存在,创建管道2\n");
        ret = mkfifo("fifo2", 0664);
        if (ret == -1)
        {
            perror("pipe");
            exit(0);
        }
    }

    // 打开管道fifo1,写管道
    int fdw = open("fifo1", O_WRONLY);
    if (fdw == -1)
    {
        perror("fifo1 fdw");
        exit(0);
    }
    printf("打开fifo1 写管道...\n");

    // 打开管道fifo2,读管道
    int fdr = open("fifo2", O_RDONLY);
    if (fdr == -1)
    {
        perror("fifo2 fdr");
        exit(0);
    }
    printf("打开fifo2 读管道...\n");
    char buf[128];
    while (1)
    {
        // 写数据
        memset(buf, 0, 128);
        fgets(buf, 128, stdin);
        ret = write(fdw, buf, strlen(buf));
        if(ret == -1){
            perror("write");
            exit(0);
        }

        // 读数据
        memset(buf, 0, 128);
        int len = read(fdr, buf, 128);
        if(len <=0 ){
            perror("read");
            break;
        }
        printf("rev data: %s\n", buf);
    }
    close(fdr);
    close(fdw);

    return 0;
}
