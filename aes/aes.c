#include "aes.h"

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, \
	     EVP_CIPHER_CTX *ctx, AES_Mode mode, AlgoType algo)
{
	int i, count, ret, def_key_len;
	unsigned char key16[16], iv16[16];
	unsigned char key32[32], iv32[32];
	unsigned char *key, *iv;
	EVP_CIPHER *type;

	switch (algo) {
	case AES_ECB_128:
		key = key16;
		iv = iv16;
		def_key_len = 16;
		type = EVP_aes_128_ecb();
		break;
	case AES_CBC_256:
		key = key32;
		iv = iv32;
		def_key_len = 32;
		type = EVP_aes_256_cbc();
		break;
	default:
		break;
	}

	count = 5;
	ret = EVP_BytesToKey(type, EVP_sha1(), salt, key_data, key_data_len, \
			     count, key, iv);
	if (ret != def_key_len) {
		return -1;
	}

	EVP_CIPHER_CTX_init(ctx);
	switch (mode) {
	case AES_Mode_Encryption:
		EVP_EncryptInit_ex(ctx, type, NULL, key, iv);
		break;
	case AES_Mode_Decryption:
		EVP_DecryptInit_ex(ctx, type, NULL, key, iv);
		break;
	default:
		break;
	}

	return 0;
}

int aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *in, int ilen, \
		unsigned char *out, int *olenp)
{
	int olen = *olenp;
	int flen = 0;

	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(e, out, &olen, in, ilen);
	EVP_EncryptFinal_ex(e, out + olen, &flen);

	*olenp = olen + flen;
	return 0;
}

int aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *in, int ilen, \
		unsigned char *out, int *olenp)
{
	int olen = *olenp;
	int flen = 0;

	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, out, &olen, in, ilen);
	EVP_DecryptFinal_ex(e, out + olen, &flen);

	*olenp = olen + flen;
	return 0;
}

