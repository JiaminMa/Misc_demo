#include "unpipc.h"

#define MAXNITEMS   1000000
#define MAXTHREADS  100

int nitems;
int buff[MAXNITEMS];

struct {
    pthread_mutex_t mutex;
    int nput;
    int nval;
} put = {
    PTHREAD_MUTEX_INITIALIZER
};

struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready;
} nready = {
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER
}; 

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
    int i, nthreads, count[MAXTHREADS];
    pthread_t tid_produce[MAXTHREADS], tid_consume;

    if (argc != 3) {
        err_quit("usage not correct");
    }

    nitems = min(atoi(argv[1]), MAXNITEMS);
    nthreads = min(atoi(argv[2]), MAXTHREADS);

    Set_concurrency(nthreads);

    for (i = 0; i < nthreads; i++) {
        count[i] = 0;
        Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }

    Pthread_create(&tid_consume, NULL, consume, NULL);
    for (i = 0 ; i < nthreads; i++) {
        Pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

    Pthread_join(tid_consume, NULL);
    exit(0);
}

void *produce(void *arg)
{
    for (;;) {
        Pthread_mutex_lock(&put.mutex);
        if (put.nput >= nitems) {
            Pthread_mutex_unlock(&put.mutex);
            return NULL;
        }
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        Pthread_mutex_unlock(&put.mutex);

        Pthread_mutex_lock(&nready.mutex);
        if (nready.nready == 0) {
            Pthread_cond_signal(&nready.cond);
        }
        nready.nready++;
        Pthread_mutex_unlock(&nready.mutex);

        *((int *)arg) += 1;
    }
}

void *consume(void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {

        Pthread_mutex_lock(&nready.mutex);
        while (nready.nready == 0) {
            Pthread_cond_wait(&nready.cond, &nready.mutex);
        }
        nready.nready--;
        Pthread_mutex_unlock(&nready.mutex);

        if (buff[i] != i) {
            printf("buff[%d] = %d\n", i , buff[i]);
        }
    }
    return NULL;
}
