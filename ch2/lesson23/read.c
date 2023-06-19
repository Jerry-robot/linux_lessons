
/*
    创建方式:
    1、通过命令 mkfifo
    2、通过函数
  #include <sys/types.h>
       #include <sys/stat.h>

       int mkfifo(const char *pathname, mode_t mode);
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

    int fd = open("test", O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    while (1)
    {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(len == 0){
            printf("写端全部关闭");
            break;
        }
        printf("rec data : %s \n", buf);
    }
    close(fd);
    return 0;
}
