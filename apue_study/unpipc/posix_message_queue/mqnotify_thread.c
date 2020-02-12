#include "unpipc.h"

mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval);

int main(int argc, char **argv)
{
    if (argc != 2) {
        err_quit("usage not correct");
    }

    mqd = Mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    Mq_getattr(mqd, &attr);

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = NULL;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;
    Mq_notify(mqd, &sigev);
    
    for (;;) {
        pause();
    }

    exit(0);
}

static void notify_thread(union sigval arg)
{
    ssize_t n;
    void *buf;

    printf("notify thread start\n");
    buf = Malloc(attr.mq_msgsize);
    Mq_notify(mqd, &sigev);

    while ((n = Mq_receive(mqd, buf, attr.mq_msgsize, NULL)) >= 0) {
        printf("read %ld bytes\n", (long)n);
    }

    if (errno != EAGAIN) {
        err_sys("mq_receice error");
    }

    free(buf);
    pthread_exit(NULL);
}
