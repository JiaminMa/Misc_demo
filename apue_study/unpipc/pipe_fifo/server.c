#include "unpipc.h"

void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAXLINE];

    if ((n = Read(readfd, buff, MAXLINE)) == 0) {
        err_quit("end of file while reading pathname");
    }

    buff[n] = '\0';

    if ((fd = open(buff, O_RDONLY)) < 0) {
        snprintf(buff + n, sizeof(buff) - n, ":can't open, %s\n", strerror(errno));
        n = strlen(buff);
        Write(writefd, buff, n);
    } else {
        while ((n = Read(fd, buff, MAXLINE)) > 0) {
            Write(writefd, buff, n);
        }
        Close(fd);
    }
}
