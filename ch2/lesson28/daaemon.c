
/*
    写一个守护进程，每隔两秒获取系统时间，将系统时间写入到磁盘文件中

*/
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void work(int num)
{
    // 捕捉到信号之后，获取系统时间，写入磁盘文件
    time_t tm = time(NULL);
    // 转换格式
    struct tm *loc = localtime(&tm);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    char *str = asctime(loc);
    write(fd, str, strlen(str));

    // char buf[1024];
    // sprintf(buf, "%d-%d-%d %d:%d:%d\n", loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);
    // printf("%s\n", buf);
}

int main(int argc, char *argv[])
{
    // 1、创建子进程，杀死父进程
    pid_t pid = fork();

    // 2、退出父进程
    if (pid > 0)
    {
        exit(0);
    }
    // 2、将子进程重新创建一个会话
    if (pid == 0)
    {
        setsid();
        // 3、设置掩码
        umask(022);
        // 4、更改工作目录
        chdir("/home/gjw/");

        // 5、关闭、重定向文件描述符
        int fd = open("/dev/null", O_RDWR);

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDERR_FILENO);

        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = work;
        sigemptyset(&act.sa_mask);
        sigaction(SIGALRM, &act, NULL);

        // 6、业务逻辑
        struct itimerval val;
        val.it_interval.tv_sec = 2;
        val.it_interval.tv_usec = 0;
        val.it_value.tv_sec = 2;
        val.it_value.tv_usec = 0;

        setitimer(ITIMER_REAL, &val, NULL);
        while (1)
        {
            sleep(10);
        }
        printf("run over!");
    }
    return 0;
}
