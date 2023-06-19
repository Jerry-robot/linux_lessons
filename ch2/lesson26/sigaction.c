
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myalarm(int num){
    printf("捕捉到信号的编号：%d\n", num);
    printf("xxxxxxxxx\n");
}

int main(int argc, char *argv[])
{
    // signal(SIGALRM, SIG_IGN);
    // signal(SIGALRM, myalarm);
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    // 清空零时信号阻塞信号集
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    if(ret == -1){
        perror("itimer");
        exit(0);
    }
    puts("定时器开始！");

    // getchar();
    while (1)
    {
        /* code */
    }
    



    return 0;
}

