#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    char line[200];
    FILE *fpin;
    
    if ((fpin = popen("./myuclc", "r")) == NULL) {
        printf("popen error\n");
        exit(-1);
    }

    for (;;) {
        fputs("prompt >", stdout);
        fflush(stdout);
        if (fgets(line, 200, fpin) == NULL) {
            break;
        }

        if (fputs(line, stdout) == EOF) {
            printf("fputs error to pipe\n");
            exit(-1);
        }
    }

    pclose(fpin);
    putchar('\n');
    exit(0);
}
