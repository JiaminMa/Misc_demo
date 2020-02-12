#include "unpipc.h"

int main(int argc, char **argv)
{
    size_t n;
    char buff[MAXLINE];
    char command[MAXLINE];
    FILE *fp;

    Fgets(buff, MAXLINE, stdin);
    n = strlen(buff);
    if (buff[n - 1] == '\n') {
        n--;
    }

    snprintf(command, sizeof(command), "cat %s", buff);
    fp = Popen(command, "r");

    while (Fgets(buff, MAXLINE, fp) != NULL) {
        Fputs(buff, stdout);
    }

    Pclose(fp);
    exit(0);
}
