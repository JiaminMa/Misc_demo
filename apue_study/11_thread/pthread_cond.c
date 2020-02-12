#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

uint32_t i = 0;

pthread_cond_t ready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    for(;;) {
        pthread_mutex_lock(&lock);
        while (i % 3 != 0) {
            pthread_cond_wait(&ready, &lock);
        }
        printf("%s:i:%d\n", __func__, i);
        pthread_mutex_unlock(&lock);
    }
}

void *thread2(void *arg)
{
    for(;;) {
        pthread_mutex_lock(&lock);
        i++;
        pthread_mutex_unlock(&lock);
        sleep(1);
        pthread_cond_signal(&ready);
    }
}

int main()
{
    pthread_t tid1, tid2;
    void *tret;
    
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
