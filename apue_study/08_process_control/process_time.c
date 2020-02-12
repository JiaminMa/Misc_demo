#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>

void sys_err(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

static void pr_time(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

int main(int argc, char *argv[])
{
    int i;
    setbuf(stdout, NULL);
    for(i = 1; i < argc; i++) {
        do_cmd(argv[i]);
    }

    exit(0);
}

static void do_cmd(char *cmd)
{
    struct tms tmstart, tmsend;
    clock_t start, end;
    int status;

    printf("\ncommand: %s\n", cmd);
    if ((start = times(&tmstart)) == -1) {
        sys_err("time error");
    }

    if ((status = system(cmd)) < 0) {
        sys_err("system() error");
    }

    if ((end = times(&tmsend)) == -1) {
        sys_err("times error");
    }
    pr_time(end - start, &tmstart, &tmsend);
    pr_exit(status);
}

static void pr_time(clock_t real, struct tms *tmstart, struct tms *tmsend)
{
    static long clktck = 0;
    if (clktck == 0) {
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {
            sys_err("sysconf error");
        }
    }

    printf("real: %7.2f\n", real / (double) clktck);
    printf("user: %7.2f\n", (tmsend->tms_utime - tmstart->tms_utime) / (double) clktck);
    printf("sys: %7.2f\n", (tmsend->tms_stime - tmstart->tms_stime) / (double) clktck);
    printf("child user: %7.2f\n", (tmsend->tms_cutime - tmstart->tms_cutime) / (double) clktck);
    printf("child sys: %7.2f\n", (tmsend->tms_cstime - tmstart->tms_cstime) / (double) clktck);

}
