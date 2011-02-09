CC=gcc
CFLAGS=-g -O2

all: odirectread fileread

%.o: %.c
	$(CC) $(CFLAGS) $< -c
