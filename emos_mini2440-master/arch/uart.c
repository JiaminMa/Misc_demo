#include "uart.h"

drvstus_t hal_uart_write(unsigned int uart,void* buf,unsigned int len)
{
    char* p=buf;
    cpuflg_t cpuflg;
    hal_disableirq_savecpuflg(&cpuflg);
    
    while(*p)
    {
          if(uart_send_char(uart,*p)==DFCERRSTUS)
          {
                
                hal_sysdie("uart err");
                return DFCERRSTUS;
          }
          p++;
    }
    hal_enableirq_restcpuflg(&cpuflg);
    return DFCOKSTUS;
}

drvstus_t uart_send_char(unsigned int uart,char ch)
{
    unsigned int time=0;
    if(uart!=0)
    {
        return  DFCERRSTUS;
    }
    while(!(hal_io32_read(UTRSTAT0_R)&4))
    {
        if(time>0x100000)
        {
            return DFCERRSTUS;
        }
        time++;    
    }
	if (ch == '\n') {
		uart_send_char(uart, '\r');
	}
    hal_io32_write(UTXH0_R,ch);
    
    return DFCOKSTUS;
}

void hal_uart0_putc(char c)
{
    while(!(hal_io32_read(UTRSTAT0_R)&4));

    hal_io32_write(UTXH0_R,c);
    return;
}
