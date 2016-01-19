global entrypoint   ; the entry point symbol defined in kernel.ld

extern runKernel    ; Kernel main function

; Values for the multiboot header
MULTIBOOT_HEADER_MAGIC     equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS     equ 0x0

; magic + checksum + flags must equal 0
MULTIBOOT_HEADER_CHECKSUM  equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

; Size of the stack in bytes
STACK_SIZE  equ 262144  ; 256 KB

;---------------------------------------------------------------------------------------------------
; bootloader section
; This section must be located at the very beginning of the kernel image.

section .bootloader

; Minimum multiboot header
dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd MULTIBOOT_HEADER_CHECKSUM

entrypoint:
    ; Bootloader code starts executing here
    cli  ; disable hardware interrupts

    ; Stack initialization
    mov esp, stack
    add esp, STACK_SIZE
    mov ebp, esp

    ; Calling the kernel main function
    call runKernel

    ; Infinite loop (should never get here)
.forever:
    hlt
    jmp .forever

;---------------------------------------------------------------------------------------------------
; stack section

section .stack

stack:
resb STACK_SIZE

