CC=gcc
CFLAGS=-g -O2
EXTRALIBS=-lpthread -lcurl

all: pullweb

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

pullweb: pullweb.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf all