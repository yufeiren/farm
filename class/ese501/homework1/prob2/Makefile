CC=g++
CFLAGS=-g -O2 -I/usr/local/systemc-2.2/include/
EXTRALIBS=-L/usr/local/systemc-2.2/lib-linux64/ -lsystemc -lm

all: communication


%.o: %.cpp
	$(CC) $(CFLAGS) $< -c

communication: main.cpp communication.o test.o
	$(CC) $(CFLAGS) -o $@ main.cpp communication.o test.o $(EXTRALIBS)

clean:
	rm -rf communication *.o
