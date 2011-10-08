CC=gcc
CFLAGS=-O2 -g
EXTRALIBS=

all: multiway

%.o: %.c
	$(CC) $(CFLAGS) $< -c

multiway: main.o multiway.o
	$(CC) $(CFLAGS) -o $@ main.o multiway.o $(EXTRALIBS)

clean:
	rm -rf multiway *.o
