CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread -libverbs

all: mqp
# odirectread fileread ffileread odirectwrite filewrite ffilewrite

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

mqp: mqp.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf all