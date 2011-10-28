CC=gcc
CFLAGS=-O2
EXTRALIBS=

all: vertical

%.o: %.c
	$(CC) $(CFLAGS) $< -c

vertical: main.o vertical.o iom.o
	$(CC) $(CFLAGS) -o $@ main.o vertical.o iom.o $(EXTRALIBS)

clean:
	rm -rf vertical *.o
