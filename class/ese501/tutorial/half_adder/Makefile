CC=g++
CFLAGS=-g -O2 -I/usr/local/systemc-2.2/include/
EXTRALIBS=-L/usr/local/systemc-2.2/lib-linux64/ -lsystemc -lm

all: half_adder


%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

half_adder: main.cpp half_adder.o driver.o
	$(CC) $(CFLAGS) -o $@ main.cpp half_adder.o driver.o $(EXTRALIBS)

clean:
	rm -rf half_adder *.o
