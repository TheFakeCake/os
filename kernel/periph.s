;--------------------------------------------------------------------
; File          : periph.s
; Authors       : Benjamin Ganty, Thierry Mourao
; Date          : 4 november 2014
; Description   : Implementation of the read/write functions for peripheral registers.
;--------------------------------------------------------------------

global outb
global outw
global inb
global inw

section .text:  ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

;--------------------------------------------------------------------
outb:
    push	ebp
	mov		ebp, esp

    mov     dx, [esp+8]
    mov     al, [esp+12]
    out     dx, al
    
    mov		esp, ebp
	pop		ebp
    ret
;--------------------------------------------------------------------
outw:
    push	ebp
	mov		ebp, esp

    mov     dx, [esp+8]
    mov     ax, [esp+12]
    out     dx, ax
    
    mov		esp, ebp
	pop		ebp
    ret
;--------------------------------------------------------------------
inb:
    push	ebp
	mov		ebp, esp

    mov     dx, [esp+8]
    in      al, dx
    
    mov		esp, ebp
	pop		ebp
    ret
;--------------------------------------------------------------------
inw:
    push	ebp
	mov		ebp, esp

    mov     dx, [esp+8]
    in      ax, dx
    
    mov		esp, ebp
	pop		ebp
    ret

