TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf
LDFLAGS 	:= -g

objs := smc_start.o smc_test.o serial.o

smc_test.bin: $(objs)
	${LD} -T smc_test.ld -o smc_test.elf $^
	${OBJCOPY} -O binary -S smc_test.elf $@
	${OBJDUMP} -D -m arm smc_test.elf > smc_test.dis

run: $(objs)
	${LD} -T smc_test.ld -o smc_test.elf $^
	${OBJCOPY} -O binary -S smc_test.elf smc_test.bin
	${OBJDUMP} -D -m arm smc_test.elf > smc_test.dis
	qemu-system-arm -M vexpress-a9 -m 256M -nographic -kernel smc_test.bin

debug: $(objs)
	${LD} -T smc_test.ld -o smc_test.elf $^
	${OBJCOPY} -O binary -S smc_test.elf smc_test.bin
	${OBJDUMP} -D -m arm smc_test.elf > smc_test.dis
	qemu-system-arm -M vexpress-a9 -m 256M -nographic -kernel smc_test.elf -s -S

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC}  $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
