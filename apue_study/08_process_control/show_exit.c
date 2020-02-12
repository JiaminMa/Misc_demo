#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void sys_err(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

int main()
{
    pid_t pid;
    int status;

    printf("main entry\n");
    if ((pid = fork()) < 0){
        sys_err("fork error");
    } else if (pid == 0){
        exit(7);
    }

    if (wait(&status) != pid) {
        sys_err("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0){
        sys_err("fork error");
    } else if (pid == 0){
        abort();
    }

    if (wait(&status) != pid) {
        sys_err("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0){
        sys_err("fork error");
    } else if (pid == 0){
        status /= 0;
    }

    if (wait(&status) != pid) {
        sys_err("wait error");
    }
    pr_exit(status);

    printf("main exit\n");
    exit(0);
}
