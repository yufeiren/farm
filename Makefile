CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: odirectread fileread ffileread odirectwrite filewrite ffilewrite

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

odirectread: calspd.o calspd.h
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)
	
calspd.o: calspd.c calspd.h
	$(CC) $(CFLAGS) $< -c