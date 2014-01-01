#ifndef AES_H
#define AES_H

/* openssl-devel */
#include <openssl/evp.h>
#include <openssl/aes.h>

typedef enum AES_Mode {
	AES_Mode_Encryption,
	AES_Mode_Decryption,
	AES_Mode_Unknow
} AES_Mode;

typedef enum AlgoType {
	AES_ECB_128,
	AES_CBC_256,
	AES_NONE
} AlgoType;

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, \
	     EVP_CIPHER_CTX *ctx, AES_Mode mode, AlgoType algo);

int aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *in, int ilen, \
		unsigned char *out, int *olenp);

int aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *in, int ilen, \
		unsigned char *out, int *olenp);

#endif

