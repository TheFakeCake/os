#ifndef _GDT_H_
#define _GDT_H_

#include "../common/types.h"
#include "task.h"

#define MAX_NB_TASKS 8
#define TASKS_FIRST_GDT_ENTRY   4
#define TASKS_MEMORY_SIZE 	    0x100000
#define TASKS_MEMORY_OFFSET     0x800000

// Structure of a GDT descriptor. There are 2 types of descriptors: segments and TSS.
// Section 3.4.5 of Intel 64 & IA32 architectures software developer's manual describes
// segment descriptors while section 6.2.2 describes TSS descriptors.
typedef struct __attribute__((packed)) gdt_entry_st {
    uint16_t lim15_0;
    uint16_t base15_0;
    uint8_t base23_16;

    uint8_t type : 4;
    uint8_t s : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;

    uint8_t lim19_16 : 4;
    uint8_t avl : 1;
    uint8_t l : 1;
    uint8_t db : 1;
    uint8_t granularity : 1;

    uint8_t base31_24;
} gdt_entry_t;


typedef struct __attribute__((packed)) task_st {
	tss_t 		tss;
	gdt_entry_t ldt[2];
	uint32_t	addr;
	uint8_t 	kernel_stack[65536];
	uint32_t	tss_selector;
	uint32_t	ldt_selector;
	uint8_t		free;
} task_t;

// Structure describing a pointer to the GDT descriptor table.
// This format is required by the lgdt instruction.
typedef struct gdt_ptr_st {
    uint16_t limit;    // Limit of the table (ie. its size)
    uint32_t base;     // Address of the first entry
} __attribute__((packed)) gdt_ptr_t;

extern void gdt_init();
extern void gdt_flush(gdt_ptr_t *gdt_ptr);
extern task_t* get_task(uint32_t tss_selector);
extern int exec_task(char *fileName);

#endif
