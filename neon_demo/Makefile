TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf
LDFLAGS 	:= -g

objs := neon_start.o neon_mem.o neon_test.o serial.o

neon_test.bin: $(objs)
	${LD} -Tneon_test.ld -o neon_test.elf $^
	${OBJCOPY} -O binary -S neon_test.elf $@
	${OBJDUMP} -D -m arm neon_test.elf > neon_test.dis

run: $(objs)
	${LD} -T neon_test.ld -o neon_test.elf $^
	${OBJCOPY} -O binary -S neon_test.elf neon_test.bin
	${OBJDUMP} -D -m arm neon_test.elf > neon_test.dis
	qemu-system-arm -M realview-pb-a8 -m 256M -nographic -kernel neon_test.bin

debug: $(objs)
	${LD} -T neon_test.ld -o neon_test.elf $^
	${OBJCOPY} -O binary -S neon_test.elf neon_test.bin
	${OBJDUMP} -D -m arm neon_test.elf > neon_test.dis
	qemu-system-arm -M realview-pb-a8 -m 256M -nographic -kernel neon_test.elf -s -S

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
#	${CC}  -march=armv7-a -mfpu=neon -mfloat-abi=hard -c -o $@ $<
	${CC} $(CFLAGS) -march=armv7-a -mfpu=neon -mfloat-abi=hard -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
