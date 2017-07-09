volatile unsigned int * const UART0DR = (unsigned int *)0x10009000;

void board_putc(const char c) 
{
	*UART0DR = (unsigned int)(c);
}

void board_puts(const char *s)
{
    while(*s != '\0') {
		board_putc(*s++);
	}
}

void board_putbin(const unsigned int num)
{
	char tmp = 0;
	int flag = 31;
	for (; flag >= 0; flag--) {
		tmp = (num & (1 << flag)) == (1 << flag) ? '1' : '0';
		board_putc(tmp);
	}
	board_putc('\n');
}

static int tiny_pow(int num, int pow)
{
	int i = 0;
	int ret = 1;
	for (; i < pow; i++) {
		ret *= num;
	}
	return ret;
}

void board_puthex(const unsigned int num)
{
	int i, j;
	int flag = 31;
	int tmp = 0;
	int sum = 0;
	for (j = 7; j >= 0; j--) {
		for (i = 3; i >= 0; i--) {
			tmp = (num & (1 << flag)) == (1 << flag) ? 1 : 0; 
			sum += tmp == 1 ? tiny_pow(2, i) : 0;
			flag--;
		}
		if (sum >= 0 && sum <= 9) {
			sum = sum + '0';
		} else {
			sum = sum - 10 + 'a';
		}
		board_putc((char)sum);
		sum = 0;
	}
//	board_putc('\n');
}



