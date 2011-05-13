# `mysql_config --cflags --libs`

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2 -I../include
EXTRALIBS=-L../libkw -lkw -lxml2 -lcurl `mysql_config --cflags --libs`

all: pullrss parserss

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

pullrss: pullrss.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

parserss: parserss.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf *.o a.out core temp.* pullrss
