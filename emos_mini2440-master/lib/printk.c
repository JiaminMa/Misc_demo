#include "printk.h"
#include "cpuctrl.h"

void printk(const char* fmt,...) {
	char buf[512];
	cpuflg_t cpuflg;
	va_list ap;
	va_start(ap, fmt);
	hal_disableirq_savecpuflg(&cpuflg);

	vsprintfk(buf,fmt,ap);
	hal_uart_write(0, buf, 0);
	hal_enableirq_restcpuflg(&cpuflg);
	va_end(ap);
	return;
}

char* strcopyk(char* buf,char* str_s) {
	while(*str_s) {
		*buf=*str_s;
		buf++;
		str_s++;
	}
	return buf;
}

void vsprintfk(char* buf,const char* fmt,va_list args) {
	char* p =buf;
	va_list next_arg=args;
	while(*fmt) {
		if(*fmt != '%') {
			*p++ = *fmt++;
			continue;
		}
		fmt++;
		switch(*fmt) {
		case 'x':
			p=numberk(p,va_arg(next_arg,unsigned int), 16);
			fmt++;

			break;
		case 'd':
			p=numberk(p,va_arg(next_arg,unsigned int), 10);
			fmt++;
			break;
		case 's':
			p=strcopyk(p,(char*)va_arg(next_arg,unsigned int));
			fmt++;
			break;
		default:
			break;
		}
	}
	*p=0;
	return;
}

char* numberk(char* str,unsigned int n, int base) {
	register char *p;
	char strbuf[36];
	p = &strbuf[36];
	*--p = 0;
	if(n == 0) {
		*--p = '0';
	} else {
		do {
			*--p = "0123456789abcdef"[n % base];
		} while( n /= base);
	}
	while(*p!=0) {
		*str++=*p++;
	}
	return str;
}

void char_write_uart(char* buf) {

	char*p=buf;
	while(*p) {

		hal_uart0_putc(*p);
		p++;
	}
	return;
}

void debug(const char* fmt,...) {
#ifdef DEBUG
	char buf[512];
	cpuflg_t cpuflg;
	va_list ap;
	va_start(ap, fmt);
	hal_disableirq_savecpuflg(&cpuflg);

	vsprintfk(buf,fmt,ap);
	hal_uart_write(0, buf, 0);
	hal_enableirq_restcpuflg(&cpuflg);
	va_end(ap);
#endif
}

