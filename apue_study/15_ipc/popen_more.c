#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PAGER "${PAGER:-more}"

int main(int argc, char *argv[])
{
    char line[200];
    FILE *fpin, *fpout;

    fpin = fopen(argv[1], "r");
    if (fpin == NULL) {
        printf("cannot open %s", argv[1]);
        exit(-1);
    }

    if ((fpout = popen(PAGER, "w")) == NULL) {
        printf("popen error\n");
        exit(-1);
    }

    while (fgets(line, 200, fpin) != NULL) {
        if (fputs(line, fpout) == EOF) {
            printf("fputs error to pipe\n");
            exit(-1);
        }
    }

    pclose(fpout);
    exit(0);
}

