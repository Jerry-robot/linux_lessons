#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
struct Node
{
    int num;
    struct Node *next;
};

struct Node *head = NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *product(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("product : %ld, add num : %d\n", pthread_self(), newNode->num);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        usleep(3000);
    }
    return NULL;
}

void *consume(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        struct Node *tmpNode = head;
        // if (head != NULL)
        // {
        //     head = head->next;
        //     printf("consume : %ld, del num : %d\n", pthread_self(), tmpNode->num);
        //     free(tmpNode);
        //     pthread_mutex_unlock(&mutex);
        //     usleep(3000);
        // }else{
        //     pthread_cond_wait(&cond, &mutex);
        //     pthread_mutex_unlock(&mutex);
        // }

        while (head == NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        assert(head!=NULL);
        head = head->next;
        printf("consume : %ld, del num : %d\n", pthread_self(), tmpNode->num);
        free(tmpNode);
        pthread_mutex_unlock(&mutex);
        usleep(3000);

    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
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
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
    return 0;
}
