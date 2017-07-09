TOOL_CHAIN = arm-none-eabi-
#CC = ${TOOL_CHAIN}gcc
#AS = ${TOOL_CHAIN}as
#LD = ${TOOL_CHAIN}ld
CC = armcc
AS = armasm
LD = armalink
#OBJCOPY = ${TOOL_CHAIN}objcopy
OBJCOPY = fromelf
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= 
LDFLAGS 	:= -g

objs := start.o s_protect.o

sov_test.bin: $(objs)
	${LD} -T sov_test.ld -o sov_test.elf $^
	${OBJCOPY} -O binary -S sov_test.elf $@
#	${OBJDUMP} -D -m arm sov_test.elf > sov_test.dis

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC}  $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin 
