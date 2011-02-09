CC=gcc
CFLAGS=-g -O2

all: odirectread fileread ffileread odirectwrite filewrite ffilewrite

%.o: %.c
	$(CC) $(CFLAGS) $< -c

odirectread: calspd.o