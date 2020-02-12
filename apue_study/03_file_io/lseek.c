#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)     
char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

void err_sys(char *s)
{
    printf("%s\n", s);
    exit(-1);
}

int main()
{
    int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("create_error");
    }

    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }
    
    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }

    exit(0);
}
