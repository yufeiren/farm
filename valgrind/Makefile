CC=gcc
CFLAGS=-g -O0
EXTRALIBS=-lpthread

all: example
# odirectread fileread ffileread odirectwrite filewrite ffilewrite

# valgrind --leak-check=yes ./example

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

example: example.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf example