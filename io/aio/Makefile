CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread -laio

all: aiocp aioread


%.o: %.c
	$(CC) $(CFLAGS) $< -c

aiocp: aiocp.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

aioread: aioread.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf *.o aiocp