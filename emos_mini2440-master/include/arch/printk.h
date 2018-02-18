#ifndef _PRINTFK_H
#define _PRINTFK_H 
#include "uart.h"
typedef __builtin_va_list va_list;
#define va_start(ap,np) __builtin_va_start(ap,np)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap,ty) __builtin_va_arg(ap,ty)
#define DEBUG
void printk(const char* fmt,...);
char* strcopyk(char* buf,char* str_s);
void vsprintfk(char* buf,const char* fmt,va_list args);
char* numberk(char* str,unsigned int n, int base);
void char_write_uart(char* buf);
void debug(const char* fmt,...);
#endif
