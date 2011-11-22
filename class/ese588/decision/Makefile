CC=gcc
CFLAGS=-Wall -O2 -g -DDEBUG
#CFLAGS=-Wall -O2
EXTRALIBS=-lm

all: dt

%.o: %.c
	$(CC) $(CFLAGS) $< -c

dt: main.o dt.o iom.o
	$(CC) $(CFLAGS) -o $@ main.o dt.o iom.o $(EXTRALIBS)

clean:
	rm -rf dt *.o
