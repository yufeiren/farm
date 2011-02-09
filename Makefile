CC=gcc
CFLAGS=-g -O2

all: odirectread

%.o: %.c
	$(CC) $(CFLAGS) $< -c
