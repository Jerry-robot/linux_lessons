/*
        #include <sys/time.h>

        int getitimer(int which, struct itimerval *curr_value);
        int setitimer(int which, const struct itimerval *new_value,
                    struct itimerval *old_value);
*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 5;
    new_value.it_interval.tv_usec = 0;
    new_value.it_value.tv_sec = 5;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    if(ret == -1){
        perror("itimer");
        exit(0);
    }
    puts("定时器开始！");

    getchar();



    return 0;
}

