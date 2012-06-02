# Set this to your installation directory.
bindir = /usr/local/bin

# Uncomment this if you have the standard ANSI/ISO C header files.
# STDC_HDRS = -DSTDC_HEADERS

# Uncomment this if you have utime.h.
# UTIME_H = -DHAVE_UTIME_H

# Uncomment this if utime (FILE, NULL) works on your system.
# UTIME_NULL = -DHAVE_UTIME_NULL

# Uncomment this if struct utimbuf is defined in utime.h.
# UTIMBUF = -DHAVE_STRUCT_UTIMBUF

CC = gcc
CFLAGS = -g -O2

ALL_CFLAGS = $(STDC_HDRS) $(UTIME_H) $(UTIME_NULL) $(UTIMBUF) $(CFLAGS)

all: poke

poke: poke.o
	$(CC) -o poke $(ALL_CFLAGS) $(LDFLAGS) poke.o

.c.o:
	$(CC) -c $(ALL_CFLAGS) poke.c

install: poke
	cp poke $(bindir)/poke

clean:
	rm poke poke.o

