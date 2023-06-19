#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    // 在fork创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    // 创建子进程
    pid_t pid = fork();
    if(pid > 0){
        // 父进程
        // 从管道的读取端读取数据
        char buf[1024] = {0};
        // 关闭写端
        close(pipefd[1]);

        while (1)
        {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv : %s, pid : %d\n", buf, getpid());

            // char * str = "hello, i am parent";
            // write(pipefd[1], str, strlen(str));
            // sleep(1);
        }
        

    }else if(pid == 0){
        // 子进程
        printf("i am child process %d\n", getpid());
        char buf[1024] = {0};
        close(pipefd[0]);
        while (1)
        {
            char * str = "hello, i am child";
            write(pipefd[1], str, strlen(str));
            // sleep(1);
            // int len = read(pipefd[0], buf, sizeof(buf));
            // printf("child recv : %s, pid : %d\n", buf, getpid());

        }
        


    }

    return 0;
}



