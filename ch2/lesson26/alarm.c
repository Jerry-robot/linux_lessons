

/*
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);
        - 功能：设置定时器，到时间发送信号SIGALRM
        - 参数：
            seconds: 倒计时的时长
                        取消倒计时 alarm(0)
        - 返回值：返回倒计时剩余时间
*/
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    int seconds = alarm(2);
    printf("seconds : %d\n", seconds);
    sleep(1);

    seconds = alarm(10);
    printf("seconds : %d\n", seconds);
    while (1)
    {
        
    }
    


    return 0;
}
