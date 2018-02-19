%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
;================================
;GDT
;================================
GDT_BASE:		dd	0x00000000
				dd	0x00000000
CODE_DESC:		dd	0x0000FFFF
				dd 	DESC_CODE_HIGH4
DATA_STACK_DESC:dd	0x0000FFFF
				dd	DESC_DATA_HIGH4
VIDEO_DESC:		dd	0x80000007
				dd 	DESC_VIDEO_HIGH4
GDT_SIZE	equ		$ - GDT_BASE
GDT_LIMIT	equ		GDT_SIZE - 1
		times 60 dq 0

;================================
;GDT Selector
;================================
SELECTOR_CODE equ (0x0001 << 3) + TI_GDT + RPL0
SELECTOR_DATA equ (0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

;================================
total_mem_bytes dd 0
gdt_ptr	dw	GDT_LIMIT
		dd	GDT_BASE
ards_buf times 244 db 0
ards_nr dw 0

;================================
;Loader Entry
;================================
loader_start:
  		xor ebx, ebx
   		mov edx, 0x534d4150
   		mov di, ards_buf
.e820_mem_get_loop:
   		mov eax, 0x0000e820
   		mov ecx, 20
   		int 0x15
   		jc .mem_get_ok
   		add di, cx
   		inc word [ards_nr]
  		 cmp ebx, 0
   		jnz .e820_mem_get_loop

   		mov cx, [ards_nr]
   		mov ebx, ards_buf
   		xor edx, edx
.find_max_mem_area:
	   	mov eax, [ebx]	      ;base_add_low
		add eax, [ebx+8]	      ;length_low
		add ebx, 20
		cmp edx, eax
		jge .next_ards
		mov edx, eax		      ;edx=Total memory size
.next_ards:
   		loop .find_max_mem_area
   		jmp .mem_get_ok

.mem_get_ok:
  		 mov [total_mem_bytes], edx	 ;0xb00

;--------Enter Protect Mode------
;Step1:Enable A20
		in al, 0x92
		or al, 0000_0010B
		out 0x92, al
;Step2:Load GDT
		lgdt	[gdt_ptr]
;Step3;Enable CR0 PE
		mov eax, cr0
		or eax, 0x00000001
		mov cr0, eax

		jmp dword SELECTOR_CODE:p_mode_start

.error_hlt:
   hlt
[bits 32]
p_mode_start:
	   	mov ax, SELECTOR_DATA
	   	mov ds, ax
	   	mov es, ax
	   	mov ss, ax
	   	mov esp,LOADER_STACK_TOP
	   	mov ax, SELECTOR_VIDEO
	   	mov gs, ax

		;copy kernel.bin to memory
		mov eax, KERNEL_START_SECTOR
		mov ebx, KERNEL_BIN_BASE_ADDR
		mov ecx, 200
		call rd_disk_m32

		call setup_page
		sgdt [gdt_ptr]
		mov ebx, [gdt_ptr + 2]
		;move video memory to 3g
		or	dword [ebx + 0x18 + 4], 0xc0000000
		;move gdt to 3g
		add dword [gdt_ptr + 2], 0xc0000000
		;move stack pointer to 3g
		add esp, 0xc0000000

		;-----enable page----------
		mov eax, PAGE_DIR_TABLE_POS
		mov cr3, eax

		mov eax, cr0
		or eax, 0x80000000
		mov cr0, eax

		lgdt [gdt_ptr]

		mov byte [gs:160], 'h'
	   	jmp SELECTOR_CODE:enter_kernel

enter_kernel:
		call kernel_init
		mov esp, 0xc009f000
		jmp KERNEL_ENTRY_POINT

;=================================
;Create PDT and PT
;	Page Dir Table
;0x101000--------------
;		 |	0x100007  |
;0x100ffc--------------
;		 |		.     |
;		 |		.     |
;		 |	0x103006  |
;0x100c08--------------
;		 |	0x102007  |
;0x100c04--------------
;		 |	0x101007  |
;0x100c00--------------
;		 |		.     |
;		 |		.     |
;0x100004--------------
;		 | 0x101007   |
;0x100000--------------
;0x10100 is 1st page table pyh addr
;=================================
setup_page:

;--------Clear the page dir table------
;page dir table has 1024 PDEs
;one PDE is 4 bytes, so the PDT's size
;is 4K, clear this page dir table
		mov ecx, 4096
		mov esi, 0
.clear_page_dir:
		mov byte [PAGE_DIR_TABLE_POS + esi], 0
		inc esi
		loop .clear_page_dir

;---------create PDE---------------------
.create_pde:
		mov eax, PAGE_DIR_TABLE_POS
		add eax, 0x1000				;1st Page Phy Addr
		mov ebx, eax				;ebx is 1st Page Base Addr

;give the 1st page 3 properties
		or eax, PG_US_U | PG_RW_W | PG_P
		mov [PAGE_DIR_TABLE_POS + 0x0], eax
		mov [PAGE_DIR_TABLE_POS + 0xc00], eax

sub eax, 0x1000
;last pde:0x100ffc
		mov [PAGE_DIR_TABLE_POS + 4092], eax

;---create PTE----------
		mov ecx, 256		; 1M(memory size) / 4K(page size) = 256 entry
		mov esi, 0
		mov edx, PG_US_U | PG_RW_W | PG_P

;first page table addr: 0x101000
.create_pte:
		mov [ebx + esi * 4], edx
		add edx, 4096 				;Add one page
		inc esi
		loop .create_pte

;create kernel pde
		mov eax, PAGE_DIR_TABLE_POS
		add eax, 0x2000
		or eax, PG_US_U | PG_RW_W | PG_P
		mov ebx, PAGE_DIR_TABLE_POS
		mov ecx, 254
		mov esi, 769

;addr:0xc04
.create_kernel_pde:
		mov [ebx + esi * 4], eax
		inc esi,
		add eax, 0x1000
		loop .create_kernel_pde

		ret
;=====================================
;Read hardisk n sector
;=====================================
rd_disk_m32:
		mov esi,eax
		mov di,cx
		;Step1: setup the sector number
		mov dx,0x1f2
		mov al,cl
		out dx,al
		mov eax,esi
		;Setp2:setup the LBA addr

		mov dx,0x1f3
		out dx,al

		mov cl,8
		shr eax,cl
		mov dx,0x1f4
		out dx,al

		shr eax,cl
		mov dx,0x1f5
		out dx,al

		shr eax,cl
		and al,0x0f
		or al,0xe0
		mov dx,0x1f6
		out dx,al
		;Step3: Write wr command to hardisk controller
		mov dx,0x1f7
		mov al,0x20
		out dx,al


;Step4：Check hardisk state
.not_ready:

		in al,dx
		and al,0x88
		cmp al,0x08
		jnz .not_ready
;Step5：Read data from 0x1f0 port
		mov ax, di

		mov dx, 256
		mul dx
		mov cx, ax
		mov dx, 0x1f0
.go_on_read:
		in ax,dx
		mov [ebx], ax
		add ebx, 2
		loop .go_on_read
		ret

;=============================
;mem_cpy(dst, src, size)
;=============================
mem_cpy:
		cld
		push ebp
		mov ebp, esp
		push ecx

		mov edi, [ebp + 8]
		mov esi, [ebp + 12]
		mov ecx, [ebp + 16]
		rep movsb

		pop ecx
		pop ebp
		ret

;================================
;init the kernel
;================================
kernel_init:
		xor eax, eax
		xor ebx, ebx
		xor ecx, ecx
		xor edx, edx

		mov dx, [KERNEL_BIN_BASE_ADDR + 42]
		mov ebx, [KERNEL_BIN_BASE_ADDR + 28]
		add ebx, KERNEL_BIN_BASE_ADDR
		mov cx, [KERNEL_BIN_BASE_ADDR + 44]
.each_segment:
		cmp byte [ebx + 0], PT_NULL
		je .PTNULL

		push dword [ebx + 16]
		mov eax, [ebx + 4]
		add eax, KERNEL_BIN_BASE_ADDR
		push eax
		push dword [ebx + 8]
		call mem_cpy
		add esp,12
.PTNULL:
		add ebx, edx
		loop .each_segment
		ret
