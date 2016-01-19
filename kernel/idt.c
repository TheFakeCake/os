//////////////////////////////////////////////////////////////////////////////////////////
/// \file idt.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 24 november 2015
/// \brief Implementation of the IDT functions.
//////////////////////////////////////////////////////////////////////////////////////////

#include "idt.h"

#include "x86.h"
#include "pic.h"
#include "io.h"
#include "periph.h"
#include "string.h"
#include "keyboard.h"
#include "timer.h"

// IDT
static idt_entry_t idt[IDT_SIZE];

// IDT Pointer
static idt_ptr_t idt_ptr;

// CPU context used when saving/restoring context from an interrupt
typedef struct regs_st {
    uint32_t gs, fs, es, ds;
    uint32_t ebp, edi, esi;
    uint32_t edx, ecx, ebx, eax;
    uint32_t number, error_code;
    uint32_t eip, cs, eflags, esp, ss;
} regs_t;

// Build and return an IDT entry.
// selector is the code segment selector in which resides the ISR (Interrupt Service Routine)
// offset is the address of the ISR (NOTE: for task gates, offset must be 0)
// type indicates the IDT entry type
// dpl is the privilege level required to call the associated ISR
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl)
{
	idt_entry_t entry;
    entry.offset15_0 = offset & 0xffff;
    entry.selector = selector;
    entry.reserved = 0;
    entry.type = type;
    entry.dpl = dpl;
    entry.p = 1;
    entry.offset31_16 = (offset >> 16) & 0xffff;
	return entry;
}

//////////////////////////////////////////////////////////////////////////////////////////
void exception_handler(regs_t *regs)
{
    set_colors(RED, BLACK);

    switch (regs->number)
    {
    case 0:
        printf("Exception %d : Divide Error\r\n", regs->number);
        break;
    case 1:
        printf("Exception %d : RESERVED\r\n", regs->number);
        break;
    case 2:
        printf("Exception %d : NMI Interrupt\r\n", regs->number);
        break;
    case 3:
        printf("Exception %d : Breakpoint\r\n", regs->number);
        break;
    case 4:
        printf("Exception %d : Overflow\r\n", regs->number);
        break;
    case 5:
        printf("Exception %d : BOUND Range Exceeded\r\n", regs->number);
        break;
    case 6:
        printf("Exception %d : Invalid Opcode (Undefined Opcode)\r\n", regs->number);
        break;
    case 7:
        printf("Exception %d : Device not available (No Math Coprocessor)\r\n", regs->number);
        break;
    case 8:
        printf("Exception %d : Double Fault\r\n", regs->number);
        break;
    case 9:
        printf("Exception %d : Coprocessor Segment Overrin (reserved)\r\n", regs->number);
        break;
    case 10:
        printf("Exception %d : Invalid TSS\r\n", regs->number);
        break;
    case 11:
        printf("Exception %d : Segment Not Present\r\n", regs->number);
        break;
    case 12:
        printf("Exception %d : Stack-Segment Fault\r\n", regs->number);
        break;
    case 13:
        printf("Exception %d : General Protection\r\n", regs->number);
        break;
    case 14:
        printf("Exception %d : Page Fault\r\n", regs->number);
        break;
    case 15:
        printf("Exception %d : (Intel reserved)\r\n", regs->number);
        break;
    case 16:
        printf("Exception %d : x87 FPU Floating-Point Error (Math Fault)\r\n", regs->number);
        break;
    case 17:
        printf("Exception %d : Alignment Check\r\n", regs->number);
        break;
    case 18:
        printf("Exception %d : Machine Check\r\n", regs->number);
        break;
    case 19:
        printf("Exception %d : SIMD Floating-Point Exception\r\n", regs->number);
        break;
    case 20:
        printf("Exception %d : Virtualization Exception\r\n", regs->number);
        break;
    default:
        break;
    }
    halt();
}

//////////////////////////////////////////////////////////////////////////////////////////
void irq_handler(regs_t *regs)
{
    switch (regs->number)
    {
    case 0:
        timer_handler();
        break;
    case 1:
        keyboard_handler();
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    default:
        break;
    }
    pic_eoi(regs->number);
}

//////////////////////////////////////////////////////////////////////////////////////////
void idt_init()
{
    // Sets the IDT mermory to 0
    memset(&idt, 0, sizeof(idt));

    // Initializes the IDT pointer
    idt_ptr.base = (uint32_t)&idt;
    idt_ptr.limit = sizeof(idt);

    // Exception descriptors
    idt[0] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[1] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_1, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[2] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_2, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[3] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[4] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_4, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[5] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_5, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[6] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_6, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[7] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_7, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[8] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_8, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[9] =  idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_9, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[10] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_10, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[11] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_11, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[12] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_12, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[13] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_13, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[14] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_14, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[15] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_15, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[16] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_16, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[17] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_17, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[18] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_18, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[19] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_19, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[20] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_exception_20, TYPE_INTERRUPT_GATE, DPL_KERNEL);

    // IRQ descriptors
    idt[32] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_0, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[33] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_1, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[34] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_2, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[35] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[36] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_4, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[37] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_5, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[38] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_6, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[39] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_7, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[40] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_8, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[41] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_9, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[42] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_10, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[43] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_11, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[44] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_12, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[45] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_13, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[46] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_14, TYPE_INTERRUPT_GATE, DPL_KERNEL);
    idt[47] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_irq_15, TYPE_INTERRUPT_GATE, DPL_KERNEL);

    extern void _syscall_handler();  // Implemented in syscall_asm.s
	idt[48] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_syscall_handler, TYPE_TRAP_GATE, DPL_USER);

    // Loads the IDT
    idt_load(&idt_ptr);
}
