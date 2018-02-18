BUILD_DIR = ./out
ENTRY_POINT = 0xc0001500
ASM = nasm
CC = gcc
LD = ld
LIB = -I lib/ -I lib/kernel -I lib/user -I kernel/ -I device/ \
		-I thread/ -I userprog
ASMFLAGS = -f elf
CFLAGS = -Wall $(LIB) -c -fno-stack-protector -m32  -fno-builtin \
		-Wmissing-prototypes -Wsystem-headers -Wstrict-prototypes
LDFLAGS = -Ttext $(ENTRY_POINT) -e main -m elf_i386 \
			-Map $(BUILD_DIR)/kernel.map
OBJS = $(BUILD_DIR)/main.o \
		$(BUILD_DIR)/init.o \
		$(BUILD_DIR)/interrupt.o \
		$(BUILD_DIR)/timer.o \
		$(BUILD_DIR)/kernel.o \
		$(BUILD_DIR)/print.o \
		$(BUILD_DIR)/debug.o \
		$(BUILD_DIR)/string.o \
		$(BUILD_DIR)/bitmap.o \
		$(BUILD_DIR)/memory.o  \
		$(BUILD_DIR)/thread.o \
		$(BUILD_DIR)/list.o	  \
		$(BUILD_DIR)/switch.o \
		$(BUILD_DIR)/sync.o  \
		$(BUILD_DIR)/console.o \
		$(BUILD_DIR)/keyboard.o \
		$(BUILD_DIR)/ioqueue.o \
		$(BUILD_DIR)/tss.o		\
		$(BUILD_DIR)/process.o \
		$(BUILD_DIR)/syscall-init.o \
		$(BUILD_DIR)/syscall.o \
		$(BUILD_DIR)/stdio.o	\
		$(BUILD_DIR)/ide.o		\
		$(BUILD_DIR)/stdio-kernel.o
		
build: $(BUILD_DIR)/kernel.bin
		mv $(BUILD_DIR)/kernel.bin ../
	
$(BUILD_DIR)/main.o: kernel/main.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: kernel/interrupt.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/timer.o: device/timer.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/debug.o: kernel/debug.c 
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/string.o: lib/string.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/bitmap.o: lib/kernel/bitmap.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/stdio-kernel.o : lib/kernel/stdio-kernel.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: kernel/memory.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: device/keyboard.c
	$(CC) $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/thread.o : thread/thread.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/list.o : lib/kernel/list.c
	$(CC) $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/sync.o : thread/sync.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/console.o : device/console.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/ioqueue.o : device/ioqueue.c
	$(CC) $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/tss.o : userprog/tss.c
	$(CC) $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/process.o : userprog/process.c
	$(CC) $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/syscall-init.o : userprog/syscall-init.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/stdio.o : lib/stdio.c
	$(CC) $(CFLAGS) $< -o $@		

$(BUILD_DIR)/syscall.o : lib/user/syscall.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/ide.o : device/ide.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.S
	$(ASM) $(ASMFLAGS) $< -o $@
	
$(BUILD_DIR)/print.o: lib/kernel/print.S
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/switch.o : thread/switch.S
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY : mk_dir clean all

mk_dir:
	if [[ ! -d $(BUILD_DIR) ]];then mkdir $(BUILD_DIR);fi

clean:
	cd $(BUILD_DIR) && rm -f ./*

all: mk_dir build 