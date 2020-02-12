#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main()
{
    int c;
    while ((c = getchar()) != EOF) {
        if (isupper(c)) {
            c = tolower(c);
        } 
        
        if (putchar(c) == EOF) {
            printf("output error\n");
            exit(-1);
        }

        if (c == '\n') {
            fflush(stdout);
        }
    }
    
    exit(0);
}
