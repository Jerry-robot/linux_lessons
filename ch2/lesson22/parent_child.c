#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    /*
        实现 ps aux | grep xxx 父子进程之间的通信
        子进程：ps aux ,子进程结束后，将数据发送父进程
        父进程：获取数据，过滤

        pipe()
        execlp()
        子进程标准输出stdout_fileno 重定向到管道的写端
    */

    int fd[2];
    int ret = pipe(fd);
    if(ret == -1){
        perror("pipe");
        exit(0);
   }

    pid_t pid = fork();
    if(pid>0){
        // 父进程
        close(fd[1]);
        char * buf[1024]={0};
        int len;
        while (len = read(fd[0], buf, sizeof(buf)-1)>0)
        {
            printf("%s", buf);
            memset(buf, 0, 1024);
        }
        // 回收子进程资源
        wait(NULL);        

    }else if(pid == 0){
        // 子进程
        //文件描述符的重定向 stdout_fileno->fd[1]
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        // 执行ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);

   }else{
    perror("fork");
    exit(0);
   }





    return 0;
}
