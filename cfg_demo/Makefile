TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf
LDFLAGS 	:= -g

objs := start.o boot_entry.o serial.o cfg.o libgcc.a

test.bin: $(objs)
	${LD} -Tlink.ld -o test.elf $^
	${OBJCOPY} -O binary -S test.elf $@
	${OBJDUMP} -D -m arm test.elf > test.dis

run: $(objs)
	${LD} -T link.ld -o test.elf $^
	${OBJCOPY} -O binary -S test.elf test.bin
	${OBJDUMP} -D -m arm test.elf > test.dis
	qemu-system-arm -M vexpress-a9 -m 32M -nographic -kernel test.bin

debug: $(objs)
	${LD} -T link.ld -o test.elf $^
	${OBJCOPY} -O binary -S test.elf test.bin
	${OBJDUMP} -D -m arm test.elf > test.dis
	qemu-system-arm -M vexpress-a9 -m 32M -nographic -kernel test.elf -s -S

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
#	${CC}  -march=armv7-a -mfpu=neon -mfloat-abi=hard -c -o $@ $<
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
