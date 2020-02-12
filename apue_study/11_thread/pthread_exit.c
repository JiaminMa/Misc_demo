#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    return ((void *)1);
}

void *thr_fn2(void *arg)
{
    printf("thread 2 returning\n");
    pthread_exit((void *)2);
    return ((void *)3);
}

int main()
{
    pthread_t tid1, tid2;
    void *tret;

    pthread_create(&tid1, NULL, thr_fn1, NULL);
    pthread_create(&tid2, NULL, thr_fn2, NULL);
    pthread_join(tid1, &tret);
    printf("thread 1 exit code %ld\n", (long)tret);
    pthread_join(tid2, &tret);
    printf("thread 2 exit code %ld\n", (long)tret);
    exit(0);
}
