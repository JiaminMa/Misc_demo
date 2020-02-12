#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void err_sys(char *s)
{
    printf("%s\n", s);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int val;

    if (argc != 2) {
        err_sys("usage: a.out <descriptor#>");
    }
    
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
        err_sys("fcntl error for fd");
    }

    switch (val & O_ACCMODE) {
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            err_sys("unknow access mode");
    }

    if (val & O_APPEND) {
        printf(",append");
    }

    if (val & O_NONBLOCK) {
        printf(",nonblock");
    }
    
    if (val & O_SYNC) {
        printf(",synchronous writes");
    }

    putchar('\n');
    exit(0);
}
