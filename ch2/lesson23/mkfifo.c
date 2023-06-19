
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
int main(int argc, char *argv[])
{

  int ret = access("fifo1", F_OK);
  if (ret == -1)
  {
    printf("管道不存在，创建管道");
    ret = mkfifo("fifo1", 0664);
    if (ret == -1)
    {
      perror("fifo");
      exit(0);
    }
  }
  return 0;
}
