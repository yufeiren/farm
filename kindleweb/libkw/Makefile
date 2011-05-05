# `mysql_config --cflags --libs`

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2
EXTRALIBS=-lxml2 -lcurl `mysql_config --cflags --libs`

all: parse_nytimes

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

parse_nytimes: parse_nytimes.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf parse_nytimes