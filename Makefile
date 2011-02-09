CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread

all: odirectread fileread ffileread
# odirectwrite filewrite ffilewrite

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

odirectread: calspd.c calspd.h
	$(CC) $(CFLAGS) -o $@ $(EXTRALIBS) $@.c calspd.c

fileread: calspd.c calspd.h
	$(CC) $(CFLAGS) -o $@ $(EXTRALIBS) $@.c calspd.c

ffileread: calspd.c calspd.h
	$(CC) $(CFLAGS) -o $@ $(EXTRALIBS) $@.c calspd.c

clean:
	rm -rf $(all)