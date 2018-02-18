[bits 32]
section .text
global switch_to

switch_to:
	push esi
	push edi
	push ebx
	push ebp

	mov eax, [esp + 20]
	mov [eax], esp   	;cur->self_kstack = esp

	;---------------Next task---------
	mov eax, [esp + 24] ;next->self_kstack
	mov esp, [eax]
	pop ebp
	pop ebx
	pop edi
	pop esi
	ret				;This ret return the eip of the thread_stack
