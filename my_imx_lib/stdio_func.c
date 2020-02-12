#include <stdarg.h>

#define MULTIPLY_DIVIDE_CAN_BE_USED
extern void dcc3_put_block(const unsigned char * data, int size);
extern void dcc3_get_block(unsigned char * data, int size);

unsigned char term_getchar(void)
{
	unsigned char imsg[4];

	dcc3_get_block(imsg,1);
	return imsg[0];
}

void term_putchar( unsigned char *ch)
{
	dcc3_put_block((unsigned char *)ch,1);
}

void term_puts(const char * buffer)
{
	int i;

	for (i = 0; buffer[i] ; i++);
	dcc3_put_block((unsigned char *) buffer, i);
}

char send_char(unsigned char *ch)
{
    term_putchar((char *)ch);
    return *ch;
}

const char hex_asc_table[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
const char upper_hex_asc_table[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

int is_dec_asc(char ch)
{
    unsigned int i;
    for (i = 0; i < 10; i++) {
        if (ch == hex_asc_table[i])
            return 1;
    }

    return 0;
}

int is_asc(char ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

int is_hex_asc(char ch)
{
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

int printf_char(char ch)
{
    unsigned char c1 = (unsigned char)'\r';
    unsigned char c2 = 0;

    c2 = (unsigned char)ch;

    send_char(&c2);

    if (((unsigned char)'\n') == c2) {
        send_char(&c1);
    }

    return 0;
}

int printf_str(char *str)
{
    while (str && (*str != (char)'\0')) {
        printf_char(*str);
        str++;
    }

    return 0;
}

int printf_hex(unsigned int val, unsigned int width)
{
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

int printf_hex_upper(unsigned int val, unsigned int width)
{
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = upper_hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

#if defined(MULTIPLY_DIVIDE_CAN_BE_USED)
int printf_dec(unsigned int val)
{
    unsigned char buf[16];
    char asc = 0;
    int i = 0;
    while (1) {
        buf[i] = val % 10;
        val = val / 10;
        i++;
        if (val == 0) {
            break;
        }
    }

    for (; i > 0; i--) {
        asc = hex_asc_table[buf[i - 1]];
        printf_char(asc);
    }

    return 0;
}
#else
const unsigned int hex_weight_value_table[] =
    { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

int printf_dec(unsigned int val)
{
    unsigned int i = 0;
    unsigned int tmp = 1, tmp_w = 1;
    char asc = 0;

    /* Figure out the digitals */
    while (1) {
        tmp = (tmp << 3) + (tmp << 1);  // tmp *= 10;
        i++;
        if (tmp > val) {
            break;
        }
    }

    if (i > 8) {
        return -1;
    }

    while (i > 0) {
        if (val >= ((hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1])) {    //<=9xxx
            tmp = 9;
            tmp_w = (hex_weight_value_table[i - 1] << 3) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 3)) {   //8xxx
            tmp = 8;
            tmp_w = hex_weight_value_table[i - 1] << 3;
        } else if (val >=
                   ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) +
                    hex_weight_value_table[i - 1])) {
            tmp = 7;
            tmp_w =
                (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1) +
                hex_weight_value_table[i - 1];
        } else if (val >=
                   ((hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1))) {
            tmp = 6;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + (hex_weight_value_table[i - 1] << 1);
        } else if (val >= ((hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1])) {
            tmp = 5;
            tmp_w = (hex_weight_value_table[i - 1] << 2) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 2)) {
            tmp = 4;
            tmp_w = hex_weight_value_table[i - 1] << 2;
        } else if (val >= ((hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1])) {
            tmp = 3;
            tmp_w = (hex_weight_value_table[i - 1] << 1) + hex_weight_value_table[i - 1];
        } else if (val >= (hex_weight_value_table[i - 1] << 1)) {
            tmp = 2;
            tmp_w = hex_weight_value_table[i - 1] << 1;
        } else if (val >= (hex_weight_value_table[i - 1])) {
            tmp = 1;
            tmp_w = hex_weight_value_table[i - 1];
        } else {
            tmp = 0;
            tmp_w = 0;
        }

        asc = hex_asc_table[tmp];
        printf_char(asc);
        i--;

        val -= tmp_w;
    }

    return 0;
}
#endif

void easy_printf(const char *fmt, ...)
{
    char c;
    unsigned int width = 0;
    va_list argptr;

    va_start(argptr, fmt);
    do {
        c = *fmt;
        if (c != '%') {
            printf_char(c);
        } else {
            while (1) {
                c = *++fmt;
                if ((c == 'd') || (c == 'x') || (c == 'X') || (c == 's') || (c == 'c')) {
                    if ((c == 'x') || (c == 'X')) {
                        if (*(fmt - 1) == '%')
                            width = 8;
                        else
                            width = *(fmt - 1) - '0';
                    }
                    break;
                }
            }

            switch (c) {
            case 'd':
                printf_dec(va_arg(argptr, int));
                break;
            case 'x':
                printf_hex((va_arg(argptr, int)), width);
                break;
            case 'X':
                printf_hex_upper((va_arg(argptr, int)), width);
                break;
            case 's':
                printf_str(va_arg(argptr, char *));
                break;
            case 'c':
                printf_char(va_arg(argptr, int));
                break;
            default:
                break;
            }
        }
        ++fmt;
    }
    while (*fmt != '\0');

    va_end(argptr);
}
