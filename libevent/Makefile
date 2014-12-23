CC=gcc
CFLAGS=-DHAVE_SETRLIMIT
LIBS=-levent

all: bench

bench: bench.c
	@echo '  ' CC $@;$(CC) $(CFLAGS) bench.c -o $@ $(LIBS)

clean:
	rm -f bench
