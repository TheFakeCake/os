ASMC=nasm
ASMFLAGS=-f elf32

CC=gcc
CFLAGS=-std=gnu99 -m32 -fno-builtin -ffreestanding -Wall -Wextra -c

MODE=normal

OBJS=bootloader.o kernel.o gdt.o gdt_asm.o ../common/string.o periph.o io.o idt.o idt_asm.o pic.o keyboard.o timer.o ide.o pfs.o syscall.o syscall_asm.o task_asm.o
KERNEL_DEPENDENCIES=

ifeq ($(MODE), test)
	CFLAGS += -D TEST
	OBJS += test.o
	KERNEL_DEPENDENCIES += test.h
endif

.PHONY: clean

kernel.elf: $(OBJS)
	ld $^ -o $@ -Tkernel.ld -melf_i386

bootloader.o: bootloader.s
	$(ASMC) $< -o $@ $(ASMFLAGS)

gdt.o: gdt.c gdt.h ../common/types.h x86.h ../common/string.h task.h task_asm.s pfs.h
	$(CC) $< -o $@ $(CFLAGS)

gdt_asm.o: gdt_asm.s const.inc
	$(ASMC) $< -o $@ $(ASMFLAGS)

kernel.o: kernel.c kernel.h idt.h gdt.h io.h pic.h timer.h x86.h keyboard.h ../common/types.h pfs.h $(KERNEL_DEPENDENCIES)
	$(CC) $< -o $@ $(CFLAGS)

../common/string.o:
	@make -C ../common string.o

keyboard.o: keyboard.c keyboard.h periph.h io.h x86.h ../common/types.h
	$(CC) $< -o $@ $(CFLAGS)

timer.o: timer.c timer.h ../common/types.h x86.h periph.h
	$(CC) $< -o $@ $(CFLAGS)

periph.o: periph.s periph.h ../common/types.h
	$(ASMC) $< -o $@ $(ASMFLAGS)

io.o: io.c io.h ../common/types.h periph.h
	$(CC) $< -o $@ $(CFLAGS)

test.o: test.c test.h io.h periph.h keyboard.h ../common/types.h pfs.h timer.h
	$(CC) $< -o $@ $(CFLAGS)

idt.o: idt.c idt.h ../common/types.h x86.h pic.h io.h timer.h
	$(CC) $< -o $@ $(CFLAGS)

idt_asm.o: idt_asm.s const.inc
	$(ASMC) $< -o $@ $(ASMFLAGS)

pic.o: pic.c pic.h periph.h
	$(CC) $< -o $@ $(CFLAGS)

ide.o: ide.c ide.h periph.h
	$(CC) $< -o $@ $(CFLAGS)

pfs.o: pfs.c pfs.h ide.h ../common/string.h ../common/types.h io.h
	$(CC) $< -o $@ $(CFLAGS)

syscall.o: syscall.c ../common/types.h ../common/syscall_nb.h io.h keyboard.h pfs.h timer.h
	$(CC) $< -o $@ $(CFLAGS)

syscall_asm.o: syscall_asm.s
	$(ASMC) $< -o $@ $(ASMFLAGS)

task_asm.o: task_asm.s
	$(ASMC) $< -o $@ $(ASMFLAGS)

clean:
	rm -f *.o kernel.elf
