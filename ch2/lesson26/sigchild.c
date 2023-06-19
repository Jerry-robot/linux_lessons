
/*

*/
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void myFun(int num)
{
    printf("捕捉到信号的编号：%d\n", num);
    // printf("xxxxxxxxx\n");
    // while (1)
    // {
    //     wait(NULL);
    // }

    while (1)
    {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0)
        {
            printf("child die, pid = %d\n", ret);
        }
        else if (ret == 0)
        {
            // 有子进程
            break;
        }
        else if (ret == -1)
        {
            // 没有子进程
            break;
        }
    }
}

int main(int argc, char *argv[])
{

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
    pid_t pid;
    for (int i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
    }
    if (pid > 0)
    {
        // 父进程
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        while (1)
        {
            printf("parent process pid : %d\n", getpid());
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        printf("child process pid : %d\n", getpid());
    }

    return 0;
}
