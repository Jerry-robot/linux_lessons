
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
int tickets = 100;

void *sellticket(void *arg){
    // 买票
    while (tickets > 0)
    {
        usleep(10000);
        printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
        tickets --;
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, &sellticket, NULL);
    pthread_create(&tid2, NULL, &sellticket, NULL);
    pthread_create(&tid3, NULL, &sellticket, NULL);

    // 回收线程资源
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_exit(NULL);

    return 0;
}
