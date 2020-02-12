#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 4
int main()
{
    char buf[MAXLINE];

    while(fgets(buf, MAXLINE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            printf("output error\n");
            exit(-1);
        }
    }

    if (ferror(stdin)) {
        printf("stdin error\n");
        exit(-1);
    }

    exit(0);
}
