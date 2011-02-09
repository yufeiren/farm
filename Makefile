CC=gcc
CFLAGS=-g -O2

all: odirectread

%.o: %.c
	$(CC) $(CFLAGS) $< -c
	
odirectread:
	$(CC) $(CFLAGS) -o $@ $@.o