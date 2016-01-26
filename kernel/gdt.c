#include "gdt.h"

#include "x86.h"
#include "../common/string.h"
#include "pfs.h"

#include "io.h"

#define GDT_INDEX_TO_SELECTOR(idx) ((idx) << 3)
#define GDT_SELECTOR_TO_INDEX(sel) ((sel) >> 3)

// GDT
static gdt_entry_t gdt[3 + 1 + MAX_NB_TASKS * 2];

// Pointer on the GDT
static gdt_ptr_t gdt_ptr;

// Tasks
static task_t tasks[MAX_NB_TASKS];

// Build and return a GDT entry given the various arguments (see Intel manuals).
static gdt_entry_t build_entry(uint32_t base, uint32_t limit, uint8_t type, uint8_t s, uint8_t db, uint8_t granularity, uint8_t dpl) {
	gdt_entry_t entry;
    // For a TSS and LDT, base is the addresse of the TSS/LDT structure
    // and limit is the size of the structure.
    entry.lim15_0 = limit & 0xffff;
    entry.base15_0 = base & 0xffff;
    entry.base23_16 = (base >> 16) & 0xff;
    entry.type = type;  // See TYPE_xxx flags
    entry.s = s;        // 1 for segments; 0 for system (TSS, LDT, gates)
    entry.dpl = dpl;    // privilege level
    entry.present = 1;  // present in memory
    entry.lim19_16 = (limit >> 16) & 0xf;
    entry.avl = 0;      // available for use
    entry.l = 0;        // should be 0 (64-bit code segment)
    entry.db = db;      // 1 for 32-bit code and data segments; 0 for system (TSS, LDT, gate)
    entry.granularity = granularity;  // granularity of the limit value: 0 = 1 byte; 1 = 4096 bytes
    entry.base31_24 = (base >> 24) & 0xff;
	return entry;
}

// Return a NULL entry.
static gdt_entry_t null_segment() {
	gdt_entry_t entry;
    memset(&entry, 0, sizeof(gdt_entry_t));
	return entry;
}

// Return a code segment specified by the base, limit and privilege level passed in arguments.
static gdt_entry_t gdt_make_code_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
    return build_entry(base, limit, TYPE_CODE_EXECREAD, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a data segment specified by the base, limit and privilege level passed in arguments.
static gdt_entry_t gdt_make_data_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
    return build_entry(base, limit, TYPE_DATA_READWRITE, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a TSS entry  specified by the TSS structure and privilege level passed in arguments.
// NOTE: a TSS entry can only reside in the GDT!
gdt_entry_t gdt_make_tss(tss_t *tss, uint8_t dpl) {
	return build_entry((uint32_t)tss, sizeof(tss_t)-1, TYPE_TSS, S_SYSTEM, DB_SYS, 0, dpl);
}

// Can only be set in the GDT!
gdt_entry_t gdt_make_ldt(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_LDT, S_SYSTEM, DB_SYS, 0, dpl);
}

uint gdt_entry_to_selector(gdt_entry_t *entry) {
	return GDT_INDEX_TO_SELECTOR(entry - gdt);
}

void setup_task(int i)
{
	tasks[i].free = 0;
	// Setup code and stack pointers
	tasks[i].tss.eip = 0;
	tasks[i].tss.esp = tasks[i].tss.ebp = TASKS_MEMORY_SIZE;  // stack pointers
}

int exec_task(char *fileName)
{
	// Searching a free task slot
	int i = 0;
	for (; i < MAX_NB_TASKS && !tasks[i].free; i++);

	if (i == MAX_NB_TASKS)
	{
		return -1;
	}

	// Copying the user program into the task memory
	if (file_read(fileName, (uint32_t*)tasks[i].memory) == -1)
	{
		return -1;
	}

	// Starting the task
	setup_task(i);
	extern void call_task(uint16_t tss_selector);
	call_task((uint16_t)tasks[i].tss_selector);

	// Task is now over
	tasks[i].free = 1;
	return 0;
}

void init_task(int i)
{
	gdt[TASKS_FIRST_GDT_ENTRY + i * 2] = gdt_make_tss(&tasks[i].tss, DPL_KERNEL);
	gdt[TASKS_FIRST_GDT_ENTRY + i * 2 + 1] = gdt_make_ldt((uint32_t)tasks[i].ldt, sizeof(tasks[i].ldt)-1, DPL_KERNEL);

	tasks[i].tss_selector = gdt_entry_to_selector(&gdt[TASKS_FIRST_GDT_ENTRY + i * 2]);
	tasks[i].ldt_selector = gdt_entry_to_selector(&gdt[TASKS_FIRST_GDT_ENTRY + i * 2 + 1]);

	tasks[i].free = 1;

	// Define code and data segments in the LDT; both segments are overlapping
	int ldt_code_idx = 0;
	int ldt_data_idx = 1;

	tasks[i].ldt[ldt_code_idx] = gdt_make_code_segment((uint32_t)tasks[i].memory, TASKS_MEMORY_SIZE / 4096, DPL_USER);  // code
	tasks[i].ldt[ldt_data_idx] = gdt_make_data_segment((uint32_t)tasks[i].memory, TASKS_MEMORY_SIZE / 4096, DPL_USER);  // data + stack

	// Initialize the TSS fields
	// The LDT selector must point to the task's LDT
	tasks[i].tss.ldt_selector = tasks[i].ldt_selector;

	// Code and data segment selectors are in the LDT
	tasks[i].tss.cs = GDT_INDEX_TO_SELECTOR(ldt_code_idx) | DPL_USER | LDT_SELECTOR;
	tasks[i].tss.ds = tasks[i].tss.es = tasks[i].tss.fs = tasks[i].tss.gs = tasks[i].tss.ss = GDT_INDEX_TO_SELECTOR(ldt_data_idx) | DPL_USER | LDT_SELECTOR;
	tasks[i].tss.eflags = 512;  // Activate hardware interrupts (bit 9)

	// Task's kernel stack
	tasks[i].tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	tasks[i].tss.esp0 = (uint32_t)(tasks[i].kernel_stack) + sizeof(tasks[i].kernel_stack);
}

// Initialize the GDT
void gdt_init()
{
	// Set the address and the size of the GDT in the pointer
	gdt_ptr.limit = sizeof(gdt);
	gdt_ptr.base = (uint32_t)&gdt;

	// Initializing the three segment descriptors in the GDT : NULL, code segment, data segment
	gdt[0] = null_segment();
	gdt[1] = gdt_make_code_segment(0, 1048575, DPL_KERNEL);
	gdt[2] = gdt_make_data_segment(0, 1048575, DPL_KERNEL);

    // Load the GDT
    gdt_flush(&gdt_ptr);

	// gdt[3] : entry for initial kernel TSS (CPU state of first task saved there)
	static uint8_t initial_tss_kernel_stack[65536]; // 64KB of stack
	static tss_t initial_tss;
	gdt[3] = gdt_make_tss(&initial_tss, DPL_KERNEL);
	memset(&initial_tss, 0, sizeof(tss_t));
	initial_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	initial_tss.esp0 = ((uint32_t)initial_tss_kernel_stack) + sizeof(initial_tss_kernel_stack);

	// Load the task register to point to the initial TSS selector.
	// IMPORTANT: The GDT must be already loaded before loading the task register!
	extern void load_task_register(uint16_t tss_selector);  // Implemented in task_asm.s
	load_task_register(gdt_entry_to_selector(&gdt[3]));

	// Init the tasks structures and descriptors
	for (int i = 0; i < MAX_NB_TASKS; i++)
	{
		init_task(i);
	}
}

task_t* get_task(uint32_t tss_selector)
{
	return &tasks[(GDT_SELECTOR_TO_INDEX(tss_selector) - TASKS_FIRST_GDT_ENTRY) / 2];
}
