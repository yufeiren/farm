CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: client server


%.o: %.c
	$(CC) $(CFLAGS) $< -c

client: client.c mysplice.o
	$(CC) $(CFLAGS) -o $@ $@.c mysplice.o $(EXTRALIBS)

server: server.c mysplice.o
	$(CC) $(CFLAGS) -o $@ $@.c mysplice.o $(EXTRALIBS)


clean:
	rm -rf client server *.o