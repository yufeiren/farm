# `mysql_config --cflags --libs`

CC=gcc
CFLAGS=-g -O2 -I/usr/include/libxml2 -I../include `mysql_config --cflags`
EXTRALIBS=
KWLIB=libkw.a

OBJS	= geturl.o rssparser.o html2text.o data2tex.o

AR=ar
RANLIB=ranlib

all:	${OBJS}
	${AR} -r ${KWLIB} ${OBJS}
	${RANLIB} ${KWLIB}

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

clean:
	rm -rf *.o a.out core temp.* ${KWLIB}