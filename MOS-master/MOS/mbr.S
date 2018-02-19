%include "boot.inc"
;Main Boot Sector
;===============================
SECTION	MBR vstart=0x7c00
		mov ax, cs
		mov ds, ax
		mov es, ax
		mov ss, ax
		mov	fs, ax
		mov sp, 7c00h
		mov ax, 0b800h
		mov gs, ax

;================================
;Clear the screen
;================================
		mov ax, 600h
		mov bx, 700h
		mov cx, 0
		mov dx, 184fh
		int 10h

;================================
;Print the string
;================================
		mov byte [gs:0x00], '3'
		mov byte [gs:0x01], 0xA4

		mov byte [gs:0x02], ' '
		mov byte [gs:0x03], 0xA4

		mov byte [gs:0x04], 'M'
		mov byte [gs:0x05], 0xA4

		mov byte [gs:0x06], 'B'
		mov byte [gs:0x07], 0xA4

		mov byte [gs:0x08], 'R'
		mov byte [gs:0x09], 0xA4

		mov eax, LOADER_START_SECTOR
		mov bx, LOADER_BASE_ADDR
		mov cx, 4
		call rd_disk_m_16

		jmp LOADER_BASE_ADDR + 0x300
;===================================
;Function: rd_disk_m_16
;Read N sectors from the hardisk
;Paramter:	eax = LBA Sector number
;		   	bx = Memory Addr to write
;			cx = Sector count
;===================================
rd_disk_m_16:
		mov esi, eax	;backup eax
		mov di,	cx		;backup cx

;Read the hardisk
;Step1:Set the Sector Count
		mov dx, 0x1f2	;0x1f2:Sector count reg
		mov al, cl
		out dx, al
		mov eax, esi	;restore the eax

;Step2:write the LBA Address to regs
;		0x1f3, 0x1f4, 0x1f5 are LBA Addr regs
		mov dx, 0x1f3
		out dx, al

		mov cl, 8
		shr eax, cl
		mov dx, 0x1f4
		out dx, al

		shr eax, cl
		mov dx, 0x1f5
		out dx, al

		shr eax, cl
		and al, 0x0f
		or al, 0xe0
		mov dx, 0x1f6
		out dx, al

;Step3:Write the wr cmd to 0x1f7, 0x20
		mov dx, 0x1f7
		mov al, 0x20
		out dx, al

;Step4:Check the hardisk status
	.not_ready:
		nop
		in al, dx
		and al, 0x88
		cmp al, 0x08
		jnz .not_ready

;Step5:Read Data from 0x1f0
		mov ax, di
		mov dx, 256
		mul dx
		mov cx, ax
		mov dx, 0x1f0
	.go_on_read:
		in ax, dx
		mov [bx], ax
		add bx, 2
		loop .go_on_read
		ret

;=========rd_disk_m_16 ENd==========
		times 510 - ($ - $$) db 0
		db 0x55, 0xaa
