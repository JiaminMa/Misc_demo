#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <stdint.h>

uint64_t count;
struct timeval end;

void sys_err(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

void checktime(char *str)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec) {
        printf("%s count  = %lld\n", str, count);
        exit(0);
    }
}

void err_quit(const char *fmt, ...)
{
}

int main(int argc, char *argv[])
{
    pid_t pid;
    char *s;
    int nzero, ret;
    int adj = 0;

    setbuf(stdout, NULL);
#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#endif

    printf("NZERO = %d\n", nzero);
    if (argc == 2) {
        adj = strtol(argv[1], NULL, 10);
    }
    gettimeofday(&end, NULL);
    end.tv_sec += 10;

    if ((pid = fork()) < 0) {
        sys_err("fork failed");
    } else  if(pid == 0) {
        s = "child";
        printf("current nice value in child is %d, adjusting by %d\n", nice(0) + nzero, adj);
        errno = 0;
        if ((ret = nice(adj) == -1) && (errno != 0) ) {
            sys_err("child set scheduling priority");
        }
        printf("now child nice value is %d\n", ret + nzero);
    } else {
        s = "paraent";
        printf("current nice value in parent is %d\n", nice(0) + nzero);
    }

    for(;;) {
        if (++count == 2000000) {
            printf("%s count  = %lld\n", s, count);
            exit(0);
        }
    }

}

