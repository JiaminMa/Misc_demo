#include "unpipc.h"

#define NBUFF 10
#define MAXTHREADS 100

int nitems, nproducers, nconsumers;

struct {
    int buff[NBUFF];
    int nput;
    int nputval;
    int nget;
    int ngetval;
    sem_t mutex, nempty, nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
    int i, count[MAXTHREADS];
    int conscount[MAXTHREADS];

    pthread_t tid_produce[MAXTHREADS];
    pthread_t tid_consume[MAXTHREADS];

    if (argc != 4) {
        err_quit("usage not correct");
    }

    nitems = atoi(argv[1]);
    nproducers = min(atoi(argv[2]), MAXTHREADS);
    nconsumers = min(atoi(argv[3]), MAXTHREADS);

    Sem_init(&shared.mutex, 0, 1);
    Sem_init(&shared.nempty, 0, NBUFF);
    Sem_init(&shared.nstored, 0, 0);

    Set_concurrency(nproducers + 1);

    for (i = 0; i < nproducers; i++) {
        count[i] = 0;
        Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }

    for (i = 0; i < nconsumers; i++) {
        conscount[i] = 0; 
        Pthread_create(&tid_consume[i], NULL, consume,& conscount[i]);
    }
    
    for (i = 0 ; i < nproducers; i++) {
        Pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

    for (i = 0; i < nconsumers; i++) {
        Pthread_join(tid_consume[i], NULL);
        printf("consumer count[%d] = %d\n", i, conscount[i]);
    }
    Sem_destroy(&shared.mutex);
    Sem_destroy(&shared.nempty);
    Sem_destroy(&shared.nstored);
    exit(0);
}

void *produce(void *arg)
{
    for (;;) {

        Sem_wait(&shared.nempty);
        Sem_wait(&shared.mutex);
        
        if(shared.nput >= nitems) {
            Sem_post(&shared.nstored);
            Sem_post(&shared.nempty);
            Sem_post(&shared.mutex);
            return NULL;
        }

        shared.buff[shared.nput % NBUFF] = shared.nputval;
        shared.nput++;
        shared.nputval++;

        Sem_post(&shared.mutex);
        Sem_post(&shared.nstored);
        *((int *)arg) += 1;
    }
}

void *consume(void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {
        Sem_wait(&shared.nstored);
        Sem_wait(&shared.mutex);

        if (shared.nget >= nitems) {
            Sem_post(&shared.nstored);
            Sem_post(&shared.mutex);
            return NULL;
        }

        i = shared.nget % NBUFF;
        if (shared.buff[i % NBUFF] != shared.ngetval) {
            printf("wrong buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        } else {
            //printf("correct buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        }
   
        shared.nget++;
        shared.ngetval++; 
        Sem_post(&shared.mutex);
        Sem_post(&shared.nempty);
        *((int *)arg) += 1;
    }
    return NULL;
}
