volatile unsigned int * const UART0DR = (unsigned int *)0x10009000;

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
          *UART0DR = (unsigned int)(*s); /* Transmit char */
           s++; /* Next char */
    }
}

void __stack_chk_fail()
{
    print_uart0("__stack_chk_fail()\n");
    while(1);
}

void *__stack_chk_guard = (void *)0;


void attack_attack()
{
    print_uart0("attack attack!\n");
}

int test_stack_overflow(int a, int b, int c, int d, int e)
{
    int i;
    int c_arr[15];
    int *p = c_arr;
    i = 15;
    c_arr[0] = 2;
    c_arr[1] = 3;
//    *(p + 15) = (int)attack_attack;
    *(p + 23) = (int)attack_attack;
    return 0;
}

int c_entry() {
    print_uart0("befroe test_stack_overflow\n");
    test_stack_overflow(1, 2, 3, 4, 5);
    print_uart0("after test_stack_overflow\n");
    return 0;
}
