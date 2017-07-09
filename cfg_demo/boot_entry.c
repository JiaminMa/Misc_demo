#include "cfg.h"
void board_puts(const char *s);
void board_puthex(const unsigned int num);

int raise(int signum)
{
    return 0;
}

typedef struct {
    unsigned char buf[8];
    void (*foo)();
}buf_ov_struct;

void foo1()
{
    board_puts("foo1\n");
}

void foo2()
{
    board_puts("foo2\n");
}

void illegal_foo()
{
    board_puts("illegal_foo\n");
}

void boot_entry() {

    unsigned char *attack_str = "\xaa\xaa\xaa\xaa"
                                "\xbb\xbb\xbb\xbb"
                                "\x74\x00\01\x60"
                                "\x02";
    unsigned char *buf;
    board_puts("c_entry!\n");
    board_puthex(CFG_BITMAP_SIZE);
    board_puts("\n"); 

    cfg_register(foo1);
    cfg_register(foo2);

    buf_ov_struct test;
    test.foo = foo1;
    cfg_check(test.foo);
    test.foo();

    test.foo = foo2;
    cfg_check(test.foo);
    test.foo();

    /*
     * Assume that the attack_str is hacker-controlled.
     * memcpy(test.name, attack_str, sizeof(attack_str))
     * */
    buf = test.buf;
    while (*attack_str != 2) {
        *buf++ = *attack_str++;
    }
    cfg_check(test.foo);
    test.foo();

    cfg_move(foo1);
    test.foo = foo1;
    cfg_check(test.foo);
    test.foo();
    for(;;);
}

