#!/bin/sh
dd if=mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc
dd if=loader.bin  of=hd60M.img bs=512 count=4 seek=2 conv=notrunc
dd if=kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc 
#dd if=kernel.bin  of=hd60M.img bs=512 count=200 seek=9 conv=notrunc
bochs -f bochsrc
