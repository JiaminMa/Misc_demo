#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n, int1, int2;
    char line[200];

    printf("add2 %s entry\n", __func__);    
    while ((n = read(STDIN_FILENO, line, 200)) > 0) {
        line[n] = 0;
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            printf("add2 read done\n");
            sprintf(line, "%d\n", int1 + int2);
            n = strlen(line);
            if (write(STDOUT_FILENO, line, n) != n) {
                printf("write error\n");
                exit(-1);
            }
        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) {
                printf("write error\n");
                exit(-1);
            }
        }
    }

    exit(0);
}
