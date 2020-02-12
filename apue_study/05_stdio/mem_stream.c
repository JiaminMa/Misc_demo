#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BSZ 48

void err_sys(char *err_msg)
{
    printf("%s\n", err_msg);
    exit(-1);
}

int main()
{
    FILE *fp;
    char buf[BSZ];

    memset(buf, 'a', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL) {
        err_sys("fmemopen failed");
    }
    printf("initial buffer conetes:%s\n", buf);
    fprintf(fp, "hello world");
    fflush(fp);
    printf("after flush: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    memset(buf, 'b', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello world");
    fseek(fp, 0, SEEK_SET);
    printf("after fseek: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    memset(buf, 'c', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello world");
    fclose(fp);
    printf("after fclose: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    return 0;
}
