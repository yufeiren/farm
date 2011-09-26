CC=g++
CFLAGS=-g -O2
EXTRALIBS=-lsystemc -lm

all: half_adder


%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

half_adder: main.o half_adder.o
	$(CC) $(CFLAGS) -o $@ main.o half_adder.o $(EXTRALIBS)

clean:
	rm -rf half_adder *.o
