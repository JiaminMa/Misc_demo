#include "unpipc.h"

struct shmstruct {
    int count;
};

sem_t *mutex;

int main(int argc, char **argv)
{
    int fd, i, nloop;
    pid_t pid;
    struct shmstruct *ptr;

    if (argc != 4) {
        err_quit("usage not correct");
    }

    nloop = atoi(argv[3]);

    fd = Shm_open(argv[1], O_RDWR, FILE_MODE);
    ptr = Mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);

    mutex = Sem_open(argv[2], 0);
    pid = getpid();
    for (i = 0; i < nloop; i++) {
        Sem_wait(mutex);
        printf("pid %ld: %d\n", (long )pid, ptr->count++);
        Sem_post(mutex);
    }
    exit(0);
}
