CC=gcc
CFLAGS=-g -O2

all: odirectread fileread ffileread

%.o: %.c
	$(CC) $(CFLAGS) $< -c
