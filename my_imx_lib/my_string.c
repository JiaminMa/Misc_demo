#include <stdint.h>
#include <stddef.h>

void memclr(uint8_t *addr, size_t size)
{
    size_t i = 0;

    if ((addr == NULL) || (size <= 0)){
        goto clean_up;
    }

    for (i = 0; i < size; i++) {
        *addr++ = 0;
    }

clean_up:
    return;
}
