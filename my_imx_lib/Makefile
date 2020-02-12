TOOL_CHAIN = aarch64-linux-gnu-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf
LDFLAGS 	:= -g

objs := start.o loader_start.o my_string.o stdio_func.o cortexa_dcc3.o imx_flexspi.o

boot.bin: $(objs)
	${LD} -Tboot.ld -o boot.elf $^
	${OBJCOPY} -O binary -S boot.elf $@
	${OBJDUMP} -D -m aarch64 boot.elf > boot.dis


%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
