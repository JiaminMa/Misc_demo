#include "unpipc.h"

#define NBUFF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

int nitems;
struct {
    int buff[NBUFF];
    sem_t *mutex;
    sem_t *nempty;
    sem_t *nstored;
} shared;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    pthread_t tid_produce, tid_consume;

    if (argc != 2) {
        err_quit("usage not correct");
    }

    nitems = atoi(argv[1]);

    shared.mutex = Sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0644, 1);
    shared.nempty = Sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, FILE_MODE, NBUFF);
    shared.nstored = Sem_open(SEM_NSTORED, O_CREAT | O_EXCL, FILE_MODE, 0);

    Set_concurrency(2);
    Pthread_create(&tid_produce, NULL, produce, NULL);
    Pthread_create(&tid_consume, NULL, consume, NULL);

    Pthread_join(tid_produce, NULL);
    Pthread_join(tid_consume, NULL);

    Sem_unlink(SEM_MUTEX);
    Sem_unlink(SEM_NEMPTY);
    Sem_unlink(SEM_NSTORED);
    exit(0);
}

void *produce(void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {
        Sem_wait(shared.nempty);
        Sem_wait(shared.mutex);
        shared.buff[i % NBUFF] = i;
        Sem_post(shared.mutex);
        Sem_post(shared.nstored);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {
        Sem_wait(shared.nstored);
        Sem_wait(shared.mutex);
        if (shared.buff[i % NBUFF] != i) {
            printf("wrong buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        } else {
            printf("correct buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        }
        Sem_post(shared.mutex);
        Sem_post(shared.nempty);
    }
    return NULL;
}

