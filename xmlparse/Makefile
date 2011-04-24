#xml2-config --cflags
#xml2-config --libs

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2
EXTRALIBS=-lxml2

all: parsesample parserss

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

parsesample: parsesample.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

parserss: parserss.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf $(all)