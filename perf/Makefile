CC=gcc
CFLAGS=-g -O2
EXTRALIBS=

all: proccpuusage

#

%.o: %.c
	$(CC) $(CFLAGS) $< -c $(EXTRALIBS)

proccpuusage: proccpuusage.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf proccpuusage