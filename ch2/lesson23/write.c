
// 向管道写数据

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // 1、判断管道是否存在
  int ret = access("test", F_OK);
  if (ret == -1)
  {
    printf("管道不存在，创建管道");
    // 2、创建管道
    ret = mkfifo("test", 0664);
    if (ret == -1)
    {
      perror("fifo");
      exit(0);
    }
  }
    // 3、打开管道
    int fd = open("test", O_WRONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    for (int i = 0; i < 100; i++)
    {
        char buf[1024] = {0};
        sprintf(buf, "hello, %d", i);
        printf("write data : %s \n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }
    close(fd);




  return 0;
}

