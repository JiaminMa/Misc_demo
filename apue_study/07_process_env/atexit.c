#include <stdio.h>
#include <stdlib.h>

void err_sys(char *str)
{
    printf("%s\n", str);
    exit(-1);
}

static void my_exit1(void)
{
    printf("%s exit handler\n", __func__);
}

static void my_exit2(void)
{
    printf("%s exit handler\n", __func__);
}

int main(void)
{
    if (atexit(my_exit2) != 0) {
        err_sys("cannot register my_exit2");
    }

    if (atexit(my_exit1) != 0) {
        err_sys("cannot register my_exit1");
    }

    if (atexit(my_exit1) != 0) {
        err_sys("cannot register my_exit1");
    }

    printf("main is done\n");
    return 0;
}
