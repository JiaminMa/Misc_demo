extern void easy_printf(const char *fmt, ...);

int main()
{

    unsigned int *add =(unsigned int *)0x0;
    *add = 1;
    add = (unsigned int *)0x1ffe0000;
    *add = 2;

    add = (unsigned int *)0x80000000;
    *add = 3;

    easy_printf("Hello Cortex M\n");
    while(1);
    return 0;
}
