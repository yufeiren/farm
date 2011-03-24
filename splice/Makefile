CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: client server


%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

client: client.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

server: server.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)


clean:
	rm -rf client server *.o