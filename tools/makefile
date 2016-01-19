CC=gcc
CFLAGS=-Wall -std=c99 -lm

all: pfscreate pfsadd pfslist pfsdel

pfscreate: pfscreate.o pfs.o
	$(CC) $^ -o $@ $(CFLAGS)

pfsadd: pfsadd.o pfs.o
	$(CC) $^ -o $@ $(CFLAGS)

pfslist: pfslist.o pfs.o
	$(CC) $^ -o $@ $(CFLAGS)

pfsdel: pfsdel.o pfs.o
	$(CC) $^ -o $@ $(CFLAGS)

pfs.o: pfs.c pfs.h
	$(CC) -c $< -o $@ $(CFLAGS)

pfscreate.o: pfscreate.c pfs.h
	$(CC) -c $< -o $@ $(CFLAGS)

pfsadd.o: pfsadd.c pfs.h
	$(CC) -c $< -o $@ $(CFLAGS)

pfslist.o: pfslist.c pfs.h
	$(CC) -c $< -o $@ $(CFLAGS)

pfsdel.o: pfsdel.c pfs.h
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o pfscreate pfsadd pfslist pfsdel
