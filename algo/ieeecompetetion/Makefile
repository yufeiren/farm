CC=gcc
CFLAGS=-O2
EXTRALIBS=

all: maze

%.o: %.c
	$(CC) $(CFLAGS) $< -c

maze: maze.c
	$(CC) $(CFLAGS) -o $@ $@.c $(EXTRALIBS)

clean:
	rm -rf maze *.o