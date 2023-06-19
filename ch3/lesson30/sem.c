#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>

struct Node
{
    int num;
    struct Node *next;
};

struct Node *head = NULL;
pthread_mutex_t mutex;
sem_t psem, csem;

void *product(void *arg)
{
    while (1)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("product : %ld, add num : %d\n", pthread_self(), newNode->num);
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        // usleep(3000);
    }
    return NULL;
}

void *consume(void *arg)
{
    while (1)
    {
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node *tmpNode = head;
        head = head->next;
        printf("consume : %ld, del num : %d\n", pthread_self(), tmpNode->num);
        free(tmpNode);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        // usleep(3000);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    pthread_t ptids[5], ctids[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&ptids[i], NULL, product, NULL);
        pthread_create(&ctids[i], NULL, consume, NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

    while (1)
    {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    // sem_destroy(&psem);
    // sem_destroy(&csem);
    pthread_exit(NULL);
    return 0;
}
