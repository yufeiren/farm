# `mysql_config --cflags --libs`

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2 -I../include
EXTRALIBS=-L../libkw -lkw -lxml2 -lcurl `mysql_config --cflags --libs`

all: parse_maweidu

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

parse_maweidu: parse_maweidu.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf *.o a.out core temp.* parse_maweidu