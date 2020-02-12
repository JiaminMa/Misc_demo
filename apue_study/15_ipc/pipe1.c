#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int n;
    int fd[2];
    pid_t pid;
    char line[48];
    
    pipe(fd);
    
    if ((pid = fork()) < 0) {
        printf("fork error\n");
        exit(-1);
    } else if (pid > 0) {
        close(fd[0]);
        sleep(2);
        write(fd[1], "hello world\n", 12);
    } else {
        close(fd[1]);
        n = read(fd[0], line, 48);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
}
