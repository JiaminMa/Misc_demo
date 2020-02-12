#ifndef MY_LOADER_H
#define MY_LOADER_H
#include <stddef.h>

extern void memclr(uint8_t *addr, size_t size);
extern void easy_printf(const char *fmt, ...);
#define trace easy_printf
#endif /* MY_LOADER_H */
