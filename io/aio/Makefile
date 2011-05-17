CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: aiocp


%.o: %.c
	$(CC) $(CFLAGS) $< -c

aiocp: aiocp.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf *.o aiocp