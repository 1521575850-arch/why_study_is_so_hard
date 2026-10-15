#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define N 5
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0; // 0:A打印，1:B打印

void *printA(void *arg)
{
    for (int i = 0; i < N; i++)
    {
        pthread_mutex_lock(&mtx);
        while (turn != 0)
        {
            pthread_cond_wait(&cond, &mtx);
        }
        printf("A");
        fflush(stdout);
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}
void *printB(void *arg)
{
    for (int i = 0; i < N; i++)
    {
        pthread_mutex_lock(&mtx);
        while (turn != 1)
        {
            pthread_cond_wait(&cond, &mtx);
        }
        printf("B");
        fflush(stdout);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}