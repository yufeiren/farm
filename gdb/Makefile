CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: hello FindPrimes


%.o: %.c
	$(CC) $(CFLAGS) $< -c

hello: hello.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

FindPrimes: Main.o CheckPrime.o
	$(CC) $(CFLAGS) -o $@ Main.o CheckPrime.o $(EXTRALIBS)

clean:
	rm -rf *.o hello