#include "console.h"
#include "print.h"
#include "sync.h"
#include "thread.h"

static struct lock console_lock;

void console_init() {
	lock_init(&console_lock);
}

void console_acquire() {
	lock_acquire(&console_lock);
}

void console_release(void) {
	lock_release(&console_lock);
}

void console_put_str(char *str)  {
	console_acquire();
	put_str(str);
	console_release();
}

void console_put_char(uint8_t val) {
	console_acquire();
	put_char(val);
	console_release();
}

void console_put_int(int val) {
	console_acquire();
	put_int(val);
	console_release();
}
