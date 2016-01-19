ASMC=nasm
ASMFLAGS=-f elf32

CC=gcc
CFLAGS=-std=gnu99 -m32 -fno-builtin -ffreestanding -Wall -Wextra -c

.PHONY: all shell shell2  tictactoe clean

all: shell shell2 tictactoe

shell: shell.o ulibc.o syscall.o app_stub.o ../common/string.o
	ld $^ -o $@ -Tapp.ld -melf_i386

tictactoe: tictactoe.o ulibc.o syscall.o app_stub.o
	ld $^ -o $@ -Tapp.ld -melf_i386

ulibc.o: ulibc.c ulibc.h ../common/types.h ../common/syscall_nb.h ../common/string.h
	$(CC) $< -o $@ -c $(CFLAGS)

shell.o: shell.c ulibc.h ../common/string.h
	$(CC) $< -o $@ -c $(CFLAGS)

tictactoe.o: tictactoe.c ulibc.h ../common/string.h
	$(CC) $< -o $@ -c $(CFLAGS)

syscall.o: syscall.s
	$(ASMC) $< -o $@ $(ASMFLAGS)

app_stub.o: app_stub.s
	$(ASMC) $< -o $@ $(ASMFLAGS)

../common/string.o:
	@make -C ../common string.o

clean:
	rm -f *.o shell
	rm -f *.o shell2
	rm -f *.o tictactoe
