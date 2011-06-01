CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: hello


%.o: %.c
	$(CC) $(CFLAGS) $< -c

hello: hello.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf *.o hello