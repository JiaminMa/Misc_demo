#ifndef _UART_H_
#define _UART_H_
#include "s3c2440.h"
#include "types.h"
#include "cpuctrl.h"

drvstus_t hal_uart_write(unsigned int uart,void* buf,unsigned int len);
drvstus_t uart_send_char(unsigned int uart, char ch);
void hal_uart0_putc(char c);
#endif

