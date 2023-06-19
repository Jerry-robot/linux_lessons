#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int num = 0;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
void *writeNum(void *arg){
    while (1)
    {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("wtid : %ld, num : %d\n", pthread_self(), num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        usleep(1000);
    }
    return NULL;

}

void *readNum(void *arg){
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        // pthread_mutex_lock(&mutex);
        printf("rtid : %ld, num : %d\n", pthread_self(), num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        usleep(1000);

    }
    return NULL;
}

int main(int argc, char *argv[])
{
    // pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t wtids[3], rtids[5];
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

        for (int i = 0; i < 3; i++)
    {
        pthread_detach(wtids[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_detach(rtids[i]);
    }

    pthread_exit(NULL);
    // pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);

    
    return 0;
}
