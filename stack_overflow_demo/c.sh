#/bin/sh

armcc s_protect.c -c -o s_protect.o --protect_stack_all  -O0
armlink --entry _Reset --ro-base 0x60010000 --output sov_test_pro.elf start.o s_protect.o
armcc s_protect.c -c -o s_protect.o --no_protect_stack -O0
armlink --entry _Reset --ro-base 0x60010000 --output sov_test_nopro.elf start.o s_protect.o
