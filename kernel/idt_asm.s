%include "const.inc"

section .text   ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

;------------------------------------------------
; CPU exceptions
%macro exception 1
global _exception_%1
_exception_%1:
    cli          ; disable interrupts
%if %1 <= 7 || %1 = 9 || %1 = 15 || %1 = 16 || %1 >= 18
    push    0    ; dummy error code
%endif
    push    %1   ; exception number
    jmp     exception_wrapper
%endmacro

%assign i 0
%rep 21
exception i
%assign i i+1
%endrep

;------------------------------------------------
; IRQ
%macro irq 1
global _irq_%1
_irq_%1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    %1   ; irq number
    jmp     irq_wrapper
%endmacro

%assign i 0
%rep 16
irq i
%assign i i+1
%endrep

;------------------------------------------------
; Wrappers for exceptions and interruptions
%macro wrapper 1
extern %1_handler

%1_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    %1_handler   ; implemented in idt.c
    pop     eax          ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call to exception_wrapper
    add     esp,8
    iret
%endmacro

wrapper exception
wrapper irq

;------------------------------------------------
; IDT Loading function
global idt_load
idt_load:
    mov     eax, [esp+4]
    lidt    [eax]
    ret
