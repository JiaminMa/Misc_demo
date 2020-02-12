#include <netinet/in.h>
#include <stdio.h>

int main()
{
    uint32_t host = 0x12345678;
    uint32_t net = htonl(host);
    printf("%x\n", net);
}
