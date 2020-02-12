#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static void sig_pipe(int);

int main()
{
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[200];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
        printf("signal error\n");
        exit(-1);
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("pipe error\n");
        exit(-1);
    }
    
    if ((pid = fork()) < 0) {
        printf("fork error\n");
        exit(-1);
    } else if (pid > 0) {
        close(fd1[0]);
        close(fd2[1]);

        while (fgets(line, 200, stdin) != NULL) {
            n = strlen(line);
            if (write(fd1[1], line, n) != n) {
                printf("write error to pipe\n");
                exit(-1);
            }
    
            printf("write done\n");

            if ((n = read(fd2[0], line, 200)) < 0) {
                printf("read error from pipe\n");
                exit(-1);
            }

            if (n == 0) {
                printf("child closed pipe\n");
                break;
            }
        }
        line[n] = 0;
        if (fputs(line, stdout) == EOF) {
            printf("fputs error\n");
            exit(-1);
        }

        exit(0);
    } else {
        close(fd1[1]);
        close(fd2[0]);

        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
                printf("dup2 error to stdin\n");
                exit(-1);
            }
            close(fd1[0]);
        }

        printf("child process2\n");
        if (fd2[1] != STDOUT_FILENO) {
            printf("child process3\n");
#if 0
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
                printf("dup2 error to stdout\n");
                exit(-1);
            }
#endif
            dup2(fd2[1], STDOUT_FILENO);
            printf("child process4\n");
            close(fd2[1]);
        }
        
        printf("before execl add2\n");
        if (execl("./add2", "add2", (char *)0) < 0) {
            printf("execl error\n");
            exit(-1);
        }
    }
    exit(0);
}

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}
