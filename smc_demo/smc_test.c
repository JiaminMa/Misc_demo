/*
 * Created by Jiamin Ma
 */
#include "smc_test.h"

extern void call_smc(void);

#define NULL ((void *)0)
#define SMC_HANDLE_MAX 16

typedef void (*smc_handle)(void);

static void smc_handle0()
{
	board_puts("######maybe rom patch can be here222####\n");
}

static void smc_handle1()
{
	board_puts("smc_handle1\n");
}

static void default_smc_handle()
{
	board_puts("default_smc_handle\n");
}

smc_handle smc_handle_arr[] = {
	smc_handle0,
	smc_handle1,
	NULL,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
	default_smc_handle,
};

void smc_handler(int smc_num)
{
	if (smc_num >= SMC_HANDLE_MAX || smc_num < 0) {
		return;
	}

	board_puts("smc_handler! smc_num is ");
	board_putc(smc_num + '0');
	board_putc('\n');
	if (smc_handle_arr[smc_num] != NULL) {
		(smc_handle_arr[smc_num])();
	} else {
		board_puts("smc handle not exist\n");
	}
	board_puts("\n");
}

void test_in_smc_in_fun()
{
	char *s = "test_in_smc_in_fun\n";
	board_puts(s);
}

void test_call_smc()
{
	board_puts("test_call_smc\n");
	call_smc();
}

void c_entry() {
	board_puts("c_entry!\n");
//	for(;;);
}

