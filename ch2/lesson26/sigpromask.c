#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    sigprocmask(SIG_BLOCK, &set, NULL);
    int num = 0;
    while (1)
    {
        num++;
        sigset_t pendingset;
        sigemptyset(&pendingset);

        int ret = sigpending(&pendingset);
        if (ret == -1)
        {
            perror("sigpend");
            continue;
        }
        for (int i = 1; i <= 32; i++)
        {
            if (sigismember(&pendingset, i) == 1)
            {
                printf("1");
            }
            else if (sigismember(&pendingset, i) == 0)
            {
                printf("0");
            }
            else
            {
                perror("sigismember");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);

        if (num == 10)
        {

            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }

    return 0;
}
