# API Note

```c
#include <netinet/in.h>

struct in_addr {
    in_addr_t s_addr;
};

/* IPv4 socket addr structure */
struct sockaddr_in {
    uint8_t sin_len;
    sa_family_t sin_family; /* AF_INET */
    in_port_t sin_port;
    
    struct in_addr sin_addr;
    char sin_zero[8];
}

#include <sys/socket.h>
/* General socket addr structure*/
struct sockaddr {
    uint8_t sa_len;
    sa_family_t sa_family;      /* AF_xxx */
    char sa_data[14];
};

int bind(int, struct sockaddr *, socklen_t);
struct sockaddr_in serv;

```
### Byte order
```c
#include <netinet/in.h>
uint16_t htons(uint16_ host16);
uint32_t htonl(uint32_t host32);

uint16_t ntohs(uint16_t net16);
uint32_t ntohl(uint32_t net32);

int inet_aton(const char *strptr, struct in_addr *addrptr);
in_addr_t inet_addr(const char *strptr);    //Deprecated
char *inet_ntoa(struct in_addr inaddr);

#include <apra/inet.h>
int inet_pton(int family, const char *strptr, void *addrptr);
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);

```
