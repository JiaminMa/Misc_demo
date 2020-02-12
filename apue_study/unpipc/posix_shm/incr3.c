#include "unpipc.h"

struct shared {
    sem_t mutex;
    int count;
} shared;

int main(int argc, char **argv)
{
    int fd, i, nloop;
    struct shared *ptr;

    if (argc != 3) {
        err_quit("usage not correct");
    }

    nloop = atoi(argv[2]);

    fd = Open(argv[1], O_RDWR | O_CREAT, FILE_MODE);
    Write(fd, &shared, sizeof(struct shared));
    ptr = Mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Close(fd);
    
    Sem_init(&ptr->mutex, 1, 1);
    setbuf(stdout, NULL);

    if (Fork() == 0) {
        for (i = 0; i < nloop; i++) {
            Sem_wait(&ptr->mutex);
            printf("child: %d\n", ptr->count++);
            Sem_post(&ptr->mutex);
        }
        exit(0);
    }

    for (i = 0; i < nloop; i++) {
        Sem_wait(&ptr->mutex);
        printf("parent: %d\n", ptr->count++);
        Sem_post(&ptr->mutex);
    }
    exit(0);
}
