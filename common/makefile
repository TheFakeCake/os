CC=gcc
CFLAGS=-std=gnu99 -m32 -fno-builtin -ffreestanding -Wall -Wextra -c

.PHONY: all clean

all: string.o

string.o: string.c string.h types.h
	$(CC) $< -o $@ -c $(CFLAGS)

clean:
	rm -f *.o
