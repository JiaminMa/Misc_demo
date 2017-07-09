/*
 * Created by Jiamin Ma
 */
typedef unsigned int size_t;
extern void board_puts(const char *s);
extern void board_puthex(unsigned int num);
void memcpy_neon(void *dest, const void *src, size_t count);
int src[32] = { 0x11111111, 0x22222222, 0x33333333, 0x44444444,
                0x55555555, 0x66666666, 0x77777777, 0x88888888,
                0x99999999, 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc,
                0xdddddddd, 0xeeeeeeee, 0xffffffff, 0x00000000,
                0x11111111, 0x22222222, 0x33333333, 0x44444444,
                0x55555555, 0x66666666, 0x77777777, 0x88888888,
                0x99999999, 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc,
                0xdddddddd, 0xeeeeeeee, 0xffffffff, 0x00000000};
int dst[32];

void *memcpy_c(void *dest, const void *src, size_t count)  
{  
    char *tmp = dest;  
    const char *s = src;  
         
    while (count--)  
        *tmp++ = *s++ ;  
           
    return dest;  
}

void memclr_c(void *addr, size_t size)
{
    char *tmp = addr;
    while (size--) {
        *tmp++ = 0;
    }
}

void dump_memory(int *mem, size_t size)
{
    size_t i = 0, j = 0;
    for (i = 0; i < size; i++) {
        board_puthex(mem[i]);
        board_puts(" ");
        j++;
        if (j == 4) {
            board_puts("\n");
            j = 0;
        }
    }
    board_puts("\n");
}

void c_entry() {
#if 0
	board_puts("c_entry!\n");

    memclr_c(dst, 4 * 32);
    board_puts("#########Before mem copy######\n");
    board_puts("Dump src memory\n");
    dump_memory(src, 32);
    board_puts("Dump dst memory\n");
    dump_memory(dst, 32);
#endif
//    memcpy_c(dst, src, 4 * 32);
    memcpy_neon(dst, src, 4 * 32);
    board_puts("#########After mem copy########\n");
    board_puts("Dump src memory\n");
    dump_memory(src, 32);
    board_puts("Dump dst memory\n");
    dump_memory(dst, 32);
    for(;;);
}

