CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lxml2

all: parsesample

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

parsesample: parsesample.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf all