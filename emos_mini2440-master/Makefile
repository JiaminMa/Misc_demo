CC = arm-linux-gcc
LD = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

OUT_DIR = ./out
CFLAGS := -I./include/ -I./include/arch -I./include/drivers -Wall

objs := arch/init.o arch/start.o arch/s3c2440.o arch/cpuctrl.o \
        arch/arm920t.o arch/vector.o arch/interrupt.o arch/uart.o\
        arch/global.o arch/mach.o arch/mm.o arch/handle_interrupt.o \
		arch/intabtdistr.o	\
        kernel/krlinit.o kernel/krlpagempol.o kernel/krlglobal.o \
		kernel/krlintupt.o 	kernel/krlwaitlist.o kernel/krlsem.o \
		kernel/krldevice.o kernel/krlthread.o kernel/krlsched.o \
		kernel/krlcpuidle.o \
        lib/printk.o lib/_umodsi3.o lib/_divsi3.o lib/_modsi3.o \
        lib/_udivsi3.o lib/div0.o \
		drivers/drvtick.o drivers/drvfs.o\
		utask/utask.o

export CC LD OBJCOPY OBJDUMP

lmos.bin: $(objs) 
	${LD} -Tlmos.lds -g -o lmos_elf $^
	${OBJCOPY} -O binary -S lmos_elf $@
	${OBJDUMP} -D -m arm lmos_elf > lmos.dis

%.o:%.c
	${CC} $(CFLAGS) -c -g -o $@ $<
	
%.o:%.s
	${CC} $(CFLAGS) -c -g -o $@ $<
	
clean:
	cd arch;rm *.o;cd ..
	cd kernel;rm *.o;cd ..
	cd lib;rm *.o;cd ..
	cd drivers;rm *.o;cd ..
	rm lmos_elf lmos.dis

