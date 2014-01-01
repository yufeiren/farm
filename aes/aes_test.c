/* gcc -Wall aes-test.c aes.c -lcrypto -o aes-test */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"

#define EN_BLK_SIZ 4096

int main(int argc, char **argv)
{
	char *key = argv[1];

	int i, iter;

	char *original[200];
	char *ciphertext[200];
	char *plaintext[200];

	EVP_CIPHER_CTX en;
	EVP_CIPHER_CTX de;

	int olen, olen2;

	for (i = 0; i < 200; i++) {
		original[i] = (char *) malloc (EN_BLK_SIZ);
		ciphertext[i] = (char *) malloc (EN_BLK_SIZ + AES_BLOCK_SIZE);
		plaintext[i] = (char *) malloc (EN_BLK_SIZ);

		memset(original[i], '\0', EN_BLK_SIZ);
		memset(ciphertext[i], '\0', EN_BLK_SIZ + AES_BLOCK_SIZE);
		memset(plaintext[i], '\0', EN_BLK_SIZ);
	}

	aes_init(key, strlen(key), NULL, &en, \
		 AES_Mode_Encryption, AES_ECB_128);
	aes_init(key, strlen(key), NULL, &de, \
		 AES_Mode_Decryption, AES_ECB_128);

	for (iter = 0; iter < 50; iter++) {
		for (i = 0; i < 200; i++) {
			aes_encrypt(&en, original[i], EN_BLK_SIZ,
				    ciphertext[i], &olen);
			aes_decrypt(&en, ciphertext[i], olen,
				    plaintext[i], &olen2);
		}
	}
}

