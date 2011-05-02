#xml2-config --cflags
#xml2-config --libs

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2
EXTRALIBS=-lxml2

all: parsesample parserss omplparse rsshtml kd

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

parsesample: parsesample.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

parserss: parserss.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

omplparse: omplparse.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

rsshtml: rsshtml.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

kd: main.c html2tex.o rss2tex.o ompl2tex.o data2tex.o geturl.o
	$(CC) $(CFLAGS) -o $@ main.c html2tex.o rss2tex.o ompl2tex.o data2tex.o geturl.o $(EXTRALIBS)

clean:
	rm -rf parsesample parserss omplparse rsshtml