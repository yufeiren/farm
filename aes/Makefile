all: aes_test

aes_test: aes.c aes_test.c
	gcc -Wall aes.c aes_test.c -lcrypto -o $@

