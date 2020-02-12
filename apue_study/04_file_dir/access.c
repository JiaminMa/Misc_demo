#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (access(argv[1], R_OK) < 0) {
        printf("access error for %s", argv[1]);
        exit(-1);
    } else {
        printf("read access ok\n");
    }
    return 0;
}
