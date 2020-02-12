#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static void f1(int, int, int, int);
static void f2(void);

static jmp_buf  jmpbuffer;
static int      globalval;

int main()
{
    int autoval;
    register int regval;
    volatile int volval;
    static int statval;

    globalval = 1;
    autoval = 2;
    regval = 3;
    volval = 4;
    statval = 5;

    if (setjmp(jmpbuffer) != 0) {
        printf("after long jump:\n");
        printf("globalval = %d, autoval = %d, volval = %d, statval = %d, regval = %d", globalval, autoval, volval, statval, regval);
        exit(0);
    }
    globalval = 95;
    autoval = 96;
    regval = 97;
    volval = 98;
    statval = 99;
    f1(autoval, regval, volval, statval);
}

static void f1(int i, int j, int k, int l)
{
    printf("globalval = %d, autoval = %d, volval = %d, statval = %d, regval = %d", globalval, i, j, k, l);
    f2();
}

static void f2()
{
    longjmp(jmpbuffer, 1);
}
