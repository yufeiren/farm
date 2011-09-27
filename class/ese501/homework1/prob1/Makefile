CC=g++
CFLAGS=-g -O2 -I/usr/local/systemc-2.2/include/
EXTRALIBS=-L/usr/local/systemc-2.2/lib-linux64/ -lsystemc -lm

all: counter


%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

counter: main.cpp count.o test.o
	$(CC) $(CFLAGS) -o $@ main.cpp count.o test.o $(EXTRALIBS)

clean:
	rm -rf counter *.o
