CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
ARM = qemu-system-arm

main.bin:main.c startup.c
		$(CC) -std=c11 -g -mcpu=cortex-m3 -mthumb -nostartfiles  --specs=nosys.specs  -T cm4.ld startup.c main.c qemu_print.c -o main
		$(OBJCOPY) -O binary -S -R .note -R .comment main main.bin
		$(OBJDUMP) -S main > main.list

QEMU: main.bin
		$(ARM) -M lm3s6965evb --kernel main.bin -nographic
DEBUG: main.bin
		$(ARM) -M lm3s6965evb -nographic  --kernel main.bin -S -s

clean: main.bin
		rm main.bin main main.list
