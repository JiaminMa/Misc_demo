#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void set_fl(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        printf("fcntl F_GETFL error\n");
        exit(-1);
    }

    val |= flags;

    if ((val = fcntl(fd, F_SETFL, 0)) < 0) {
        printf("fcntl F_SETFL error\n");
        exit(-1);
    }
}

void clr_fl(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        printf("fcntl F_GETFL error\n");
        exit(-1);
    }

    val &= ~flags;

    if ((val = fcntl(fd, F_SETFL, 0)) < 0) {
        printf("fcntl F_SETFL error\n");
        exit(-1);
    }
}

int main()
{
    return 0;
}
