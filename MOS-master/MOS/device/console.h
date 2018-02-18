#ifndef _DEVICE_CONSOLE_H_
#define _DEVICE_CONSOLE_H_

#include "stdint.h"
void console_init(void);
void console_acquire(void);
void console_release(void);
void console_put_str(char *);
void console_put_char(uint8_t);
void console_put_int(int);

#endif
